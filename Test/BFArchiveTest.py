import unittest, shutil, os, hashlib, sys
from subprocess import Popen, PIPE

def run(args):
  binary = '../../bfarchive'
  process = Popen([binary] + args, stdout=PIPE, stderr=PIPE)
  (stdout, stderr) = process.communicate();
  return (process.returncode, stdout, stderr)

def run_stdin(args, stdinput):
  binary = '../../bfarchive'
  process = Popen([binary] + args, stdin=PIPE, stdout=PIPE, stderr=PIPE)
  (stdout, stderr) = process.communicate(stdinput);
  return (process.returncode, stdout, stderr)

def read_file(path):
  with open(path, 'r') as content_file:
    return content_file.read()

def write_file(path, contents):
  with open(path, 'w') as content_file:
    return content_file.write(contents)

def hash_file(path):
  sha1 = hashlib.sha1()
  sha1.update(read_file(path))
  return sha1.hexdigest()

def reset_sandbox():
  # Delete all the old stuff
  shutil.rmtree('Sandbox', ignore_errors=True)
  os.makedirs('Sandbox')
  os.chdir('Sandbox')

  # Create the test directory for creating directory archives
  os.makedirs('Westeros')
  write_file('Westeros/davos.txt', 'davos')
  os.makedirs('Westeros/Lannisport')
  write_file('Westeros/Lannisport/jaime.txt', 'jaime')
  write_file('Westeros/Lannisport/tyrion.txt', 'tyrion')
  os.makedirs('Westeros/Winterfell')
  write_file('Westeros/Winterfell/arya.txt', 'arya')
  write_file('Westeros/Winterfell/bran.txt', 'bran')

  # Create the test file for creating single file archives
  write_file('hodor.txt', 'hodor')

def westeros_contents():
  return ['davos.txt', 'Lannisport/', 'Lannisport/jaime.txt',
          'Lannisport/tyrion.txt', 'Winterfell/', 'Winterfell/arya.txt',
          'Winterfell/bran.txt']

class TestNoArgs(unittest.TestCase):

  def test_prints_usage_when_no_args(self):
    (exitcode, stdout, stderr) = run([])
    self.assertEqual(exitcode, 1)
    self.assertIn('Usage', stdout)

class TestHelp(unittest.TestCase):

  def verifyResult(self, (exitcode, stdout, stderr)):
    self.assertEqual(exitcode, 0)
    self.assertIn('Usage', stdout)

  def test_short_arg(self):
    self.verifyResult(run(['-h']))

  def test_long_arg(self):
    self.verifyResult(run(['--help']))

class TestVersion(unittest.TestCase):

  def verifyResult(self, (exitcode, stdout, stderr)):
    self.assertEqual(exitcode, 0)
    self.assertRegexpMatches(stdout, '^[0-9]+\.[0-9]+\.[0-9]+$')

  def test_short_arg(self):
    self.verifyResult(run(['-V']))

  def test_long_arg(self):
    self.verifyResult(run(['--version']))

class TestCreateDirectoryArchive(unittest.TestCase):

  def verifySilent(self, (exitcode, stdout, stderr)):
    self.assertEqual(exitcode, 0)
    self.assertEqual(stdout, "")
    self.assertEqual(stderr, "")

  def verifyVerbose(self, (exitcode, stdout, stderr)):
    self.assertEqual(exitcode, 0)
    self.assertEqual(stderr, "")
    self.assertEqual(stdout.split('\n'), westeros_contents() + [''])

  def test_short_silent(self):
    self.verifySilent(run(['-a', 'Westeros', 'WesterosShort']))

  def test_long_silent(self):
    self.verifySilent(run(['--create', 'Westeros', 'WesterosLong']))

  def test_short_verbose(self):
    self.verifyVerbose(run(['-av', 'Westeros', 'WesterosShortVerbose']))

  def test_long_verbose(self):
    self.verifyVerbose(run(['--create', '--verbose', 'Westeros',
                                  'WesterosLongVerbose']))

  def test_z_all_archives_equal(self):
    self.assertEqual(hash_file('WesterosShort'),
                     hash_file('WesterosLong'))

    self.assertEqual(hash_file('WesterosShort'),
                     hash_file('WesterosShortVerbose'))

    self.assertEqual(hash_file('WesterosShort'),
                     hash_file('WesterosLongVerbose'))

class TestCreateFileArchive(unittest.TestCase):

  def verifyVerbose(self, (exitcode, stdout, stderr)):
    self.assertEqual(exitcode, 0)
    self.assertEqual(stderr, "")
    self.assertEqual(stdout.split('\n'), ['hodor.txt', ''])

  def test_short_silent(self):
    self.verifyVerbose(run(['-av', 'hodor.txt', 'HodorArchive']))

class TestListArchive(unittest.TestCase):

  def verifyResult(self, (exitcode, stdout, stderr)):
    self.assertEqual(exitcode, 0)
    self.assertEqual(stderr, "")
    self.assertEqual(stdout.split('\n'), westeros_contents() + [''])

  def test_short_arg_with_file(self):
    self.verifyResult(run(['-l', 'WesterosShort']))

  def test_long_arg_with_file(self):
    self.verifyResult(run(['--list', 'WesterosShort']))

  def test_short_arg_with_stdin(self):
    contents = read_file('WesterosShort')
    self.verifyResult(run_stdin(['-l', '-'], contents))

  def test_long_arg_with_stdin(self):
    contents = read_file('WesterosShort')
    self.verifyResult(run_stdin(['--list', '-'], contents))

reset_sandbox()
unittest.main()
