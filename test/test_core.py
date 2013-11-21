#!/usr/bin/env python2
import unittest
from socket import *

from common import *

class TestCore(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.listener = Listener()
        cls.runner = Runner("core")
        cls.runner.start()

        s = cls.listener.accept()
        cls.listener.close()
        cls.conn = Connection(s)

    @classmethod
    def tearDownClass(cls):
        cls.conn.close()
        cls.runner.stop()