# Copyright © 2023-2024 Tobias Erbsland https://erbsland.dev/ and EducateIT GmbH https://educateit.ch/
# According to the copyright terms specified in the file "COPYRIGHT.md".
# SPDX-License-Identifier: LGPL-3.0-or-later

import argparse
import logging
import re
import tomllib
from pathlib import Path
from typing import Optional


class ScriptError(Exception):
    pass


class MetaData:

    RE_TAGS = re.compile(R'([A-Z_]{4,16})\(([^)]*)\)\s*', re.DOTALL)

    def __init__(self, name: str, text: str, file: Path):
        self.name: str = name
        self.values: dict[str, list[str]] = {}
        for match in self.RE_TAGS.finditer(text):
            value_name = match.group(1)
            values = list(match.group(2).split())
            if value_name not in ['TAGS', 'TESTED_TARGETS', 'SKIP_BY_DEFAULT']:
                raise ScriptError(f'Unknown meta info marker "{value_name}" in file: {file}')
            if value_name in self.values:
                raise ScriptError(f'Duplicated meta info marker "{value_name}" in file: {file}')
            self.values[value_name] = values

    def build_code(self, indent: int) -> str:
        text = f'MetaData{{\n'
        text += ' ' * (indent + 4)
        text += f'"{self.name}",\n'
        text += ' ' * (indent + 4)
        text += '{'
        if 'TAGS' in self.values:
            text += ','.join([f'"{v}"' for v in self.values['TAGS']])
        text += '},\n'
        text += ' ' * (indent + 4)
        text += '{'
        if 'TESTED_TARGETS' in self.values:
            text += ','.join([f'"{v}"' for v in self.values['TESTED_TARGETS']])
        text += '},\n'
        text += ' ' * (indent + 4)
        flags = []
        if 'SKIP_BY_DEFAULT' in self.values:
            flags.append('MetaData::SkipByDefault')
        if self.name.startswith('print'):
            flags.append('MetaData::PrintMethod')
        if flags:
            text += '|'.join(flags)
        else:
            text += '0'
        text += '\n'
        text += ' ' * indent
        text += '}'
        return text


class TestMethod:

    def __init__(self, meta_data: MetaData):
        self.meta_data = meta_data

    def build_code(self, class_name: str) -> str:
        text = f'        std::make_tuple(&{class_name}::{self.meta_data.name},'
        text += self.meta_data.build_code(8)
        text += '),\n'
        return text


class TestClass:

    def __init__(self, file: Path, meta_data: MetaData, methods: list[TestMethod]):
        self.file: Path = file
        self.meta_data: MetaData = meta_data
        self.methods: list[TestMethod] = methods

    def build_code(self) -> str:
        text = f'Registration<{self.meta_data.name}> '
        text += f'test_registration{self.meta_data.name}{{\n'
        text += f'    {self.meta_data.build_code(4)},\n'
        text += '    {\n'
        for method in self.methods:
            text += method.build_code(self.meta_data.name)
        text += '    }\n'
        text += '};\n\n'
        return text


class WorkingSet:

    RE_TEST_CLASS = re.compile(
        R'((?:[A-Z_]{4,20}\([^)]*\)\s*)+)?' +
        R'class\s+(\w+Test)\s*:\s*public\s+(?:el::|erbsland::)?UnitTest', re.DOTALL)
    RE_TEST_METHOD = re.compile(
        R'((?:[A-Z_]{4,20}\([^)]*\)\s*)+)?' +
        R'void\s+((?:test|print)\w+)\(\)', re.DOTALL)

    def __init__(self):
        self.log: Optional[logging.Logger] = None
        self.verbose = False
        self.source_dir = Path()
        self.output = Path()
        self.input: list[Path] = []
        self.input_map: dict[str, Path] = {}
        self.test_classes: list[TestClass] = []

    def init_logging(self):
        """
        Initialize logging for this script.
        """
        logging.basicConfig(
            encoding='utf-8',
            level=logging.INFO)
        self.log = logging.getLogger('main')
        self.log.debug('Building unittest metadata')

    def parse_cmd_line(self):
        """
        Parse the command line arguments.
        """
        parser = argparse.ArgumentParser(
            description='Build unittest metadata.')
        parser.add_argument('source_dir', action='store', help='The directory where all unittest sources are stored.')
        parser.add_argument('output', action='store', help='The file to generate the metadata code.')
        parser.add_argument('input', nargs='+', help='All input files to parse.')
        parser.add_argument('-v', '--verbose', action='store_true')
        args = parser.parse_args()
        if args.verbose:
            self.verbose = True
        self.source_dir = Path(args.source_dir)
        if not self.source_dir.is_dir():
            raise ScriptError('Specified source directory does not exist.')
        self.output = Path(args.output)
        paths_from_input: list[Path] = list([Path(x) for x in args.input])
        paths_to_check: list[Path] = []
        for path in paths_from_input:
            if not path.is_file():
                raise ScriptError(f'Input file not found: {path}')
            if path.stat().st_size > 10_000_000:  # >10MB?
                raise ScriptError(f'Input file exceeds 10MB: {path}')
            if path.name.endswith('.toml'):
                content = path.read_text(encoding='utf-8')
                data = tomllib.loads(content)
                if not data or not isinstance(data, dict) or 'test_files' not in data:
                    raise ScriptError(f'Found TOML file with unknown structure: {path}')
                test_files = data['test_files']
                if not isinstance(test_files, list):
                    raise ScriptError(f'Found TOML file with unknown structure: {path}')
                for test_file in test_files:
                    if not isinstance(test_file, str):
                        raise ScriptError(f'Found TOML file with unknown structure: {path}')
                    path = Path(test_file)
                    if not path.is_file():
                        raise ScriptError(f'Input file from TOML not found: {path}')
                    if path.stat().st_size > 10_000_000:  # >10MB?
                        raise ScriptError(f'Input file from TOML exceeds 10MB: {path}')
                    paths_to_check.append(path)
            elif not path.name.endswith(('Test.cpp', 'Test.hpp', 'Test.h')):
                paths_to_check.append(path)
            else:
                self.log.debug(f'Skipping file: {path}')
        for path in paths_to_check:
            if path.name.endswith('Test.cpp'):
                if path.with_suffix('.hpp') in paths_to_check or path.with_suffix('.h') in paths_to_check:
                    self.log.debug(f'Skipping implementation for header: {path}')
                    continue
            self.input.append(path)

    def scan_all_files(self):
        for file in self.input:
            text = file.read_text(encoding='utf-8')
            class_match = self.RE_TEST_CLASS.search(text)
            if not class_match:
                self.log.debug(f'Found no test class. Skipping: {file}')
                continue
            test_class_name = class_match.group(2)
            ctor = f'{test_class_name}()'
            if ctor in text:
                self.log.debug(f'Found ctor in test class. Skipping: {file}')
                continue
            if 'TESTS_BEGIN(' in text or 'TEST_ENTRY(' in text:
                self.log.debug(f'Found manual test registration. Skipping: {file}')
                continue
            if 'ADD_CLASS' in text:
                raise ScriptError(f'No ctor or manual registration, but there is an ADD_CLASS macro in {file}.')
            test_class_meta_data = MetaData(test_class_name, str(class_match.group(1)), file)
            methods = []
            for method_match in self.RE_TEST_METHOD.finditer(text):
                method_meta_data = MetaData(str(method_match.group(2)), str(method_match.group(1)), file)
                methods.append(TestMethod(method_meta_data))
            self.test_classes.append(TestClass(file, test_class_meta_data, methods))

    def write_metadata(self):
        text = f'// Generated metadata\n'
        text += f'// Script: {Path(__file__).name}\n'
        text += f'//\n\n\n'
        text += '#include <erbsland/unittest/UnitTest.hpp>\n'  # Always include this header in case there are no tests
        for test_class in self.test_classes:
            text += f'#include "{test_class.file.relative_to(self.source_dir)}"\n'
        text += '\n\nusing ::erbsland::unittest::Registration;\n'
        text += 'using ::erbsland::unittest::MetaData;\n\n'
        for test_class in self.test_classes:
            text += test_class.build_code()
        text += '// EOF\n'
        if self.output.is_file():
            old_text = self.output.read_text(encoding='utf-8')
            if old_text == text:
                self.log.debug('No changes. Keep existing file.')
                return
        self.log.debug('Writing file with metadata.')
        self.output.write_text(text, encoding='utf-8')

    def run(self):
        try:
            self.init_logging()
            self.parse_cmd_line()
            self.scan_all_files()
            self.write_metadata()
            exit(0)
        except ScriptError as e:
            self.log.error(str(e))
            exit(str(e))


working_set = WorkingSet()
working_set.run()

