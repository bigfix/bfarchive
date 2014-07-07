import unittest, shutil, os, hashlib, sys, re
from subprocess import Popen, PIPE

def run(args):
  process = Popen([binary] + args, stdout=PIPE, stderr=PIPE)
  (stdout, stderr) = process.communicate();
  return (process.returncode, stdout, stderr)

def run_stdin(args, stdinput):
  process = Popen([binary] + args, stdin=PIPE, stdout=PIPE, stderr=PIPE)
  (stdout, stderr) = process.communicate(stdinput);
  return (process.returncode, stdout, stderr)

def read_file(path):
  with open(path, 'rb') as content_file:
    return content_file.read()

def write_file(path, contents):
  with open(path, 'wb') as content_file:
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

  os.utime('Westeros/davos.txt', (447841422, 447841422))

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
    self.assertTrue('Usage' in stdout)

class TestHelp(unittest.TestCase):

  def verifyResult(self, (exitcode, stdout, stderr)):
    self.assertEqual(exitcode, 0)
    self.assertTrue('Usage' in stdout)

  def test_short_arg(self):
    self.verifyResult(run(['-h']))

  def test_long_arg(self):
    self.verifyResult(run(['--help']))

class TestVersion(unittest.TestCase):

  def verifyResult(self, (exitcode, stdout, stderr)):
    self.assertEqual(exitcode, 0)
    self.assertTrue(re.search('^[0-9]+\.[0-9]+\.[0-9]+', stdout) != None)

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
    self.assertEqual(stdout.splitlines(), westeros_contents())

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
    self.assertEqual(stdout.splitlines(), ['hodor.txt'])

  def test_short_silent(self):
    self.verifyVerbose(run(['-av', 'hodor.txt', 'HodorArchive']))

class TestListArchive(unittest.TestCase):

  def verifyResult(self, (exitcode, stdout, stderr)):
    self.assertEqual(exitcode, 0)
    self.assertEqual(stderr, "")
    self.assertEqual(stdout.splitlines(), westeros_contents())

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

class TestExtractDirectoryArchive(unittest.TestCase):

  def verify_directory_archive(self, dir):
    davos = read_file(os.path.join(dir, 'davos.txt'))
    jaime = read_file(os.path.join(dir, 'Lannisport', 'jaime.txt'))
    tyrion = read_file(os.path.join(dir, 'Lannisport', 'tyrion.txt'))
    arya = read_file(os.path.join(dir, 'Winterfell', 'arya.txt'))
    bran = read_file(os.path.join(dir, 'Winterfell', 'bran.txt'))

    self.assertEqual(davos, 'davos')
    self.assertEqual(jaime, 'jaime')
    self.assertEqual(tyrion, 'tyrion')
    self.assertEqual(arya, 'arya')
    self.assertEqual(bran, 'bran')

    davos_mtime = os.path.getmtime(os.path.join(dir, 'davos.txt'))
    self.assertEqual(davos_mtime, 447841422)

  def verifySilent(self, (exitcode, stdout, stderr)):
    self.assertEqual(exitcode, 0)
    self.assertEqual(stderr, "")
    self.assertEqual(stdout, "")

  def verifyVerbose(self, (exitcode, stdout, stderr)):
    self.assertEqual(exitcode, 0)
    self.assertEqual(stderr, "")
    self.assertEqual(stdout.splitlines(), westeros_contents())

  def test_extract_short_arg_with_file(self):
    self.verifySilent(run(['-x', 'WesterosShort', 'WesterosShortOut']))
    self.verify_directory_archive('WesterosShortOut')

  def test_extract_long_arg_with_file(self):
    self.verifySilent(run(['--extract', 'WesterosShort', 'WesterosLongOut']))
    self.verify_directory_archive('WesterosLongOut')

  def test_extract_verbose(self):
    self.verifyVerbose(run(['-xv', 'WesterosShort', 'WesterosVerboseOut']))
    self.verify_directory_archive('WesterosVerboseOut')

  def test_extract_stdin(self):
    contents = read_file('WesterosShort')
    self.verifySilent(run_stdin(['-x', '-', 'WesterosStdinOut'], contents))
    self.verify_directory_archive('WesterosStdinOut')

  def test_z_extract_current_dir(self):
    self.verifySilent(run(['-x', 'WesterosShort']))
    self.verify_directory_archive('.')    

binary = sys.argv.pop()
reset_sandbox()
unittest.main()
