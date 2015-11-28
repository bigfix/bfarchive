#!/usr/bin/env python

import json

def include_file(name):
  return name.startswith('Source/') or name.startswith('Library/')

def filter_source_files(source_files):
  return [file for file in source_files if include_file(file['name'])]

with open('coverage.json') as coverage_file:
  coverage = json.load(coverage_file)

coverage['source_files'] = filter_source_files(coverage['source_files'])

with open('filtered-coverage.json', 'w') as coverage_file:
  json.dump(coverage, coverage_file)
