import unittest
import sys

from hape_test_base import HapeTestBase
from hape_libs.domain import HavenaskDomain
from hape_libs.common import HapeCommon
from hape_libs.testlib.main import test_main


class HapeCatalogTest(unittest.TestCase, HapeTestBase):
    def setUp(self):
        self.init()
        
    def tearDown(self):
        self.destory()

    def test_hape_catalog_validate(self):
        ## start
        self.assertTrue(self.hape_domain.start(HapeCommon.SWIFT_KEY))
        self.assertTrue(self.hape_domain.start(HapeCommon.HAVENASK_KEY))
        self._test_havenask_tables()
        
    def _test_havenask_tables(self):
        table = "simple"
        catalog_manager = self.hape_domain.suez_cluster.get_catalog_manager()
        self.assertTrue(catalog_manager != None)
        self.assertFalse(catalog_manager.create_table(table, 1, self.testdata_root + "/sut_catalog/error_schema.json", None))

        
test_main()
