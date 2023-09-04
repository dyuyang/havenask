#include <map>
#include <vector>

#include "autil/TimeUtility.h"
#include "iquan/common/Common.h"
#include "iquan/common/Status.h"
#include "iquan/common/Utils.h"
#include "iquan/common/catalog/TvfFunctionModel.h"
#include "iquan/config/ClientConfig.h"
#include "iquan/config/WarmupConfig.h"
#include "iquan/jni/IquanDqlResponse.h"
#include "iquan/jni/test/testlib/IquanTestBase.h"
#include "iquan/jni/test/testlib/TestUtils.h"

using namespace std;

namespace iquan {

class IquanTest : public IquanTestBase {};

TEST_F(IquanTest, testInit) {
    {
        JniConfig jniConfig;
        ASSERT_TRUE(jniConfig.isValid());
        ClientConfig sqlConfig;
        ASSERT_TRUE(sqlConfig.isValid());
        IquanPtr pIquan = TestUtils::createIquan(jniConfig, sqlConfig);
        ASSERT_TRUE(pIquan != nullptr);
    }
    {
        JniConfig jniConfig;
        jniConfig.tableConfig.summaryTableSuffix = "";
        ASSERT_FALSE(jniConfig.isValid());
        ClientConfig sqlConfig;
        ASSERT_TRUE(sqlConfig.isValid());
        IquanPtr pIquan = TestUtils::createIquan(jniConfig, sqlConfig);
        ASSERT_FALSE(pIquan != nullptr);
    }
    {
        JniConfig jniConfig;
        ASSERT_TRUE(jniConfig.isValid());
        ClientConfig sqlConfig;
        {
            CacheConfig cacheConfig;
            cacheConfig.initialCapcity = 0;
            sqlConfig.cacheConfigs["aaa"] = cacheConfig;
        }
        ASSERT_FALSE(sqlConfig.isValid());
        IquanPtr pIquan = TestUtils::createIquan(jniConfig, sqlConfig);
        ASSERT_FALSE(pIquan != nullptr);
    }
}

TEST_F(IquanTest, testUpdateIquanTable) {
    string iquanTableName
        = GET_PRIVATE_TEST_DATA_PATH_WITHIN_TEST() + string("iquan_catalog/t1.json");
    string iquanTableContent;
    Status status = Utils::readFile(iquanTableName, iquanTableContent);
    ASSERT_TRUE(status.ok()) << status.errorMessage();
    ASSERT_FALSE(iquanTableContent.empty());

    IquanPtr pIquan = TestUtils::createIquan();
    ASSERT_TRUE(pIquan != nullptr);
    status = pIquan->updateTables(iquanTableContent);
    ASSERT_TRUE(status.ok()) << status.errorMessage();

    string actualStr;
    string expectStr = R"json({"error_message":"","result":"[\"t1\"]","error_code":0})json";
    status = pIquan->listTables("default", "db1", actualStr);
    ASSERT_TRUE(status.ok()) << status.errorMessage();
    ASSERT_EQ(expectStr, actualStr);
}

TEST_F(IquanTest, testUpdateIquanLayerTable) {
    string iquanTableName
        = GET_PRIVATE_TEST_DATA_PATH_WITHIN_TEST() + string("iquan_catalog/layerTableLayers.json");
    string iquanTableContent;
    Status status = Utils::readFile(iquanTableName, iquanTableContent);
    ASSERT_TRUE(status.ok()) << status.errorMessage();
    ASSERT_FALSE(iquanTableContent.empty());

    string iquanLayerTableName
        = GET_PRIVATE_TEST_DATA_PATH_WITHIN_TEST() + string("iquan_catalog/layerTable1.json");
    string iquanLayerTableContent;
    status = Utils::readFile(iquanLayerTableName, iquanLayerTableContent);
    ASSERT_TRUE(status.ok()) << status.errorMessage();
    ASSERT_FALSE(iquanLayerTableContent.empty());

    IquanPtr pIquan = TestUtils::createIquan();
    ASSERT_TRUE(pIquan != nullptr);
    status = pIquan->updateTables(iquanTableContent);
    ASSERT_TRUE(status.ok()) << status.errorMessage();
    status = pIquan->updateLayerTables(iquanLayerTableContent);
    ASSERT_TRUE(status.ok()) << status.errorMessage();

    string actualStr;
    string expectStr
        = R"json({"error_message":"","result":"[\"layer_table_order_simple\",\"layer_table_simple_01\",\"layer_table_simple_three\",\"layer_table_simple_02\",\"layer_table_simple_03\",\"layer_table_tj_shop_02\",\"layer_table_tj_shop_01\",\"layer_table_t3\",\"layer_table_order\",\"layer_table_t4\",\"layer_table_t1\",\"layer_table_t2\"]","error_code":0})json";
    status = pIquan->listTables("default", "db1", actualStr);
    ASSERT_TRUE(status.ok()) << status.errorMessage();
    ASSERT_EQ(expectStr, actualStr);
}

TEST_F(IquanTest, testUpdateIquanFunctions) {
    string iquanFunctionName
        = GET_PRIVATE_TEST_DATA_PATH_WITHIN_TEST() + string("/iquan_catalog/udf.json");
    string iquanFunctionContent;
    Status status = Utils::readFile(iquanFunctionName, iquanFunctionContent);
    ASSERT_TRUE(status.ok()) << status.errorMessage();
    ASSERT_FALSE(iquanFunctionContent.empty());

    IquanPtr pIquan = TestUtils::createIquan();
    ASSERT_TRUE(pIquan != nullptr);
    status = pIquan->updateFunctions(iquanFunctionContent);
    ASSERT_TRUE(status.ok()) << status.errorMessage();

    string actualStr;
    string expectStr
        = R"json({"error_message":"","result":"[\"_query\",\"aitheta\",\"_foo_i64\",\"_foo_f64\",\"_contain\",\"_foo_s\",\"_model_recall\",\"_double\",\"NOW\",\"_notcontain\",\"_matchindex\",\"_concat\",\"_timestamp\",\"_time\",\"_multivalue\",\"_date\",\"_range\",\"_foo_i32\",\"_model_recall2\",\"_match_type_scorer2\",\"_float\",\"_match_type_scorer3\",\"_match_type_scorer1\",\"_foo_f32\",\"_rand\"]","error_code":0})json";
    status = pIquan->listFunctions("default", "db1", actualStr);
    ASSERT_TRUE(status.ok()) << status.errorMessage();
    ASSERT_EQ(expectStr, actualStr);
}

TEST_F(IquanTest, testUpdateTableValueFunctions) {

    IquanPtr pIquan = TestUtils::createIquan();
    ASSERT_TRUE(pIquan != nullptr);

    TvfModel model;
    model.catalogName = "default";
    model.databaseName = "db1";
    model.functionName = "tvf";
    model.functionType = "TVF";
    model.functionVersion = 300;
    model.isDeterministic = 1;
    model.functionContentVersion = "json_default_0.1";

    TvfInputTableDef inputTableDef;
    inputTableDef.autoInfer = true;

    TvfFieldDef checkType1;
    checkType1.fieldName = "i1";
    checkType1.fieldType = {false, "int32"};
    inputTableDef.checkFields.emplace_back(checkType1);

    TvfFieldDef checkType2;
    checkType2.fieldName = "i2";
    checkType2.fieldType = {false, "int32"};
    inputTableDef.checkFields.emplace_back(checkType2);

    TvfParamsDef tvfParams;
    tvfParams.tables.emplace_back(inputTableDef);
    tvfParams.scalars.push_back({false, "string"});
    tvfParams.scalars.push_back({false, "int32"});

    TvfReturnsDef returns;
    TvfOutputTableDef ouputTable;

    ouputTable.autoInfer = false;
    ouputTable.fieldNames.push_back("i1");
    ouputTable.fieldNames.push_back("i2");
    returns.outputTables.emplace_back(ouputTable);

    returns.newFields.push_back({"new_int64_1", {true, "int64"}});
    returns.newFields.push_back({"new_int32_1", {false, "int32"}});

    TvfDef tvfDef;
    tvfDef.params = tvfParams;
    tvfDef.returns = returns;
    model.functionContent.tvfs.emplace_back(tvfDef);

    TvfModels models;
    models.functions.emplace_back(model);

    Status status = pIquan->updateFunctions(models);
    ASSERT_TRUE(status.ok()) << status.errorMessage();
}

TEST_F(IquanTest, testUpdateTableValueFunctions2) {
    string ha3FunctionName
        = GET_PRIVATE_TEST_DATA_PATH_WITHIN_TEST() + string("/iquan_catalog/tvf.json");
    string tvfContent;
    Status status = Utils::readFile(ha3FunctionName, tvfContent);
    ASSERT_TRUE(status.ok()) << status.errorMessage();
    ASSERT_FALSE(tvfContent.empty());

    TvfModels tvfModels;
    status = Utils::fromJson(tvfModels, tvfContent);
    ASSERT_TRUE(status.ok()) << status.errorMessage();

    IquanPtr pIquan = TestUtils::createIquan();
    ASSERT_TRUE(pIquan != nullptr);
    status = pIquan->updateFunctions(tvfModels);
    ASSERT_TRUE(status.ok()) << status.errorMessage();

    string result;
    string expectedStr
        = R"json({"error_message":"","result":"[\"tvf\",\"multivalue_tvf\",\"input_table_no_auto_infer_tvf\"]","error_code":0})json";
    status = pIquan->listFunctions("default", "db1", result);
    ASSERT_TRUE(status.ok()) << status.errorMessage();
    ASSERT_EQ(expectedStr, result.c_str());

    {
        status = pIquan->getFunctionDetails("default", "db1", "multivalue_tvf", result);
        ASSERT_TRUE(status.ok()) << status.errorMessage();
        status
            = pIquan->getFunctionDetails("default", "db1", "input_table_no_auto_infer_tvf", result);
        ASSERT_TRUE(status.ok()) << status.errorMessage();
    }
}

TEST_F(IquanTest, testIquanCatalogInspect) {
    string catalogRootPath = GET_PRIVATE_TEST_DATA_PATH_WITHIN_TEST() + string("/iquan_catalog");
    IquanPtr pIquan = TestUtils::createIquan();
    ASSERT_TRUE(pIquan != nullptr);

    vector<string> tableList = {string("t1.json"), string("t2.json")};
    Status status = TestUtils::registerTable(pIquan, catalogRootPath, tableList);
    ASSERT_TRUE(status.ok()) << status.errorMessage();
    vector<string> functionList = {string("udf.json"), string("udaf.json"), string("udtf.json")};
    status = TestUtils::registerFunction(pIquan, catalogRootPath, functionList);
    ASSERT_TRUE(status.ok()) << status.errorMessage();

    {
        string result;
        Status status = pIquan->listCatalogs(result);
        ASSERT_TRUE(status.ok()) << status.errorMessage();
    }
    {
        string result;
        Status status = pIquan->listDatabases("default", result);
        ASSERT_TRUE(status.ok()) << status.errorMessage();
    }
    {
        string result;
        Status status = pIquan->listTables("default", "db1", result);
        ASSERT_TRUE(status.ok()) << status.errorMessage();
    }
    {
        string result;
        Status status = pIquan->listFunctions("default", "db1", result);
        ASSERT_TRUE(status.ok()) << status.errorMessage();
    }
    {
        string result;
        Status status = pIquan->getTableDetails("default", "db1", "t1", result);
        ASSERT_TRUE(status.ok()) << status.errorMessage();
    }
    {
        string result;
        Status status = pIquan->getFunctionDetails("default", "db1", "_collect", result);
        ASSERT_TRUE(status.ok()) << status.errorMessage();
    }
    {
        string result;
        Status status = pIquan->dumpCatalog(result);
        ASSERT_TRUE(status.ok()) << status.errorMessage();
    }
}

TEST_F(IquanTest, testWarmup) {
    string warmupSqlPath
        = GET_PRIVATE_TEST_DATA_PATH_WITHIN_TEST() + string("/warmup/warmup_sqls.json");
    WarmupConfig warmupConfig;
    warmupConfig.threadNum = 2;
    warmupConfig.warmupSeconds = 10;
    warmupConfig.warmupQueryNum = 1000000;
    warmupConfig.warmupFilePathList = {warmupSqlPath};

    string catalogRootPath = GET_PRIVATE_TEST_DATA_PATH_WITHIN_TEST() + string("/iquan_catalog");
    IquanPtr pIquan = TestUtils::createIquan();
    ASSERT_TRUE(pIquan != nullptr);

    vector<string> tableList = {string("t1.json"), string("t2.json")};
    Status status = TestUtils::registerTable(pIquan, catalogRootPath, tableList);
    ASSERT_TRUE(status.ok()) << status.errorMessage();
    vector<string> functionList = {string("udf.json"), string("udaf.json"), string("udtf.json")};
    status = TestUtils::registerFunction(pIquan, catalogRootPath, functionList);
    ASSERT_TRUE(status.ok()) << status.errorMessage();

    for (size_t i = 0; i < 3; i++) {
        long beginTime = autil::TimeUtility::currentTime();
        Status status = pIquan->warmup(warmupConfig);
        ASSERT_TRUE(status.ok()) << status.errorMessage();
        long endTime = autil::TimeUtility::currentTime();
        AUTIL_LOG(
            INFO, "warmup: round %lu, time use: %f ms", i, ((double)(endTime - beginTime)) / 1000);
        if (i > 0) {
            ASSERT_TRUE(((double)(endTime - beginTime)) / 1000 > 100);
        }
    }
}

TEST_F(IquanTest, testCache) {
    string catalogRootPath = GET_PRIVATE_TEST_DATA_PATH_WITHIN_TEST() + string("/iquan_catalog");
    IquanPtr pIquan = TestUtils::createIquan();
    ASSERT_TRUE(pIquan != nullptr);

    vector<string> tableList = {string("t1.json"), string("t2.json")};
    Status status = TestUtils::registerTable(pIquan, catalogRootPath, tableList);
    ASSERT_TRUE(status.ok()) << status.errorMessage();
    vector<string> functionList = {string("udf.json"), string("udaf.json"), string("udtf.json")};
    status = TestUtils::registerFunction(pIquan, catalogRootPath, functionList);
    ASSERT_TRUE(status.ok()) << status.errorMessage();

    //
    string catalogName = "default";
    string databaseName = "db1";
    string sql = "SELECT i1 FROM t1 WHERE i1 = ?";
    vector<vector<autil::legacy::Any>> dynamicParams;
    dynamicParams.push_back({});
    dynamicParams[0].push_back(autil::legacy::Any(100));

    // for jni cache
    ASSERT_EQ(0ul, pIquan->getPlanCacheKeyCount());
    {
        autil::legacy::json::JsonMap sqlParams = Utils::defaultSqlParams();
        sqlParams["iquan.plan.output.exec_params"] = string("true");
        sqlParams[IQUAN_PLAN_PREPARE_LEVEL] = string(IQUAN_JNI_POST_OPTIMIZE);
        sqlParams[IQUAN_PLAN_CACHE_ENALE] = string("false");

        {
            IquanDqlResponse response;
            PlanCacheStatus planCacheStatus;
            status = TestUtils::simpleQuery(pIquan,
                                            catalogName,
                                            databaseName,
                                            {sql},
                                            sqlParams,
                                            dynamicParams,
                                            response,
                                            planCacheStatus);
            ASSERT_TRUE(status.ok()) << status.errorMessage();

            ASSERT_FALSE(planCacheStatus.planGet);
            ASSERT_FALSE(planCacheStatus.planPut);
            ASSERT_EQ(0ul, pIquan->getPlanCacheKeyCount());
        }

        {
            sqlParams[IQUAN_PLAN_CACHE_ENALE] = string("true");

            IquanDqlResponse response;
            PlanCacheStatus planCacheStatus;
            status = TestUtils::simpleQuery(pIquan,
                                            catalogName,
                                            databaseName,
                                            {sql},
                                            sqlParams,
                                            dynamicParams,
                                            response,
                                            planCacheStatus);
            ASSERT_TRUE(status.ok()) << status.errorMessage();

            ASSERT_FALSE(planCacheStatus.planGet);
            ASSERT_TRUE(planCacheStatus.planPut);
            ASSERT_EQ(1ul, pIquan->getPlanCacheKeyCount());
        }
        {
            IquanDqlResponse response;
            PlanCacheStatus planCacheStatus;
            status = TestUtils::simpleQuery(pIquan,
                                            catalogName,
                                            databaseName,
                                            {sql},
                                            sqlParams,
                                            dynamicParams,
                                            response,
                                            planCacheStatus);
            ASSERT_TRUE(status.ok()) << status.errorMessage();

            ASSERT_TRUE(planCacheStatus.planGet);
            ASSERT_FALSE(planCacheStatus.planPut);
            ASSERT_EQ(1ul, pIquan->getPlanCacheKeyCount());
        }
    }
}

TEST_F(IquanTest, testTwoStageCache) {
    string catalogRootPath = GET_PRIVATE_TEST_DATA_PATH_WITHIN_TEST() + string("/iquan_catalog");
    IquanPtr pIquan = TestUtils::createIquan();
    ASSERT_TRUE(pIquan != nullptr);

    Status status = TestUtils::registerTable(pIquan, catalogRootPath, "layerTableLayers.json");
    ASSERT_TRUE(status.ok()) << status.errorMessage();
    status = TestUtils::registerLayerTable(pIquan, catalogRootPath, "layerTable1.json");
    ASSERT_TRUE(status.ok()) << status.errorMessage();
    vector<string> functionList = {string("udf.json"), string("udaf.json"), string("udtf.json")};
    status = TestUtils::registerFunction(pIquan, catalogRootPath, functionList);
    ASSERT_TRUE(status.ok()) << status.errorMessage();

    //
    string catalogName = "default";
    string databaseName = "db1";
    string sql = "SELECT id, modify_time FROM layer_table_tj_shop_01 WHERE ts > ? AND id > ? AND ? "
                 "> ts AND title = ? AND (md5 = ? OR md5 = ? OR ? = md5)";
    string sql2
        = "SELECT id, modify_time FROM layer_table_tj_shop_01 WHERE ts > 0 AND id > ? AND 90 > ts "
          "AND title = ? AND (md5 = '123456' OR md5 = '-123456' OR 'whatareyoulookingat' = md5)";
    string sql3 = "SELECT id, modify_time FROM layer_table_t1 WHERE id > ?";
    string sql4 = "SELECT id, modify_time FROM layer_table_t1 WHERE id > 0";
    string sql5 = "SELECT id, modify_time FROM layer_table_order WHERE id > 0";

    vector<vector<autil::legacy::Any>> dynamicParams;
    vector<autil::legacy::Any> dynamicParam {0,
                                             10,
                                             90,
                                             string("hello"),
                                             string("123456"),
                                             string("-123456"),
                                             string("whatareyoulookingat")};
    dynamicParams.push_back(dynamicParam);

    vector<vector<autil::legacy::Any>> dynamicParams2;
    vector<autil::legacy::Any> dynamicParam2 {10, string("hello")};
    dynamicParams2.push_back(dynamicParam2);

    // for jni cache
    ASSERT_EQ(0ul, pIquan->getPlanCacheKeyCount());
    {
        autil::legacy::json::JsonMap sqlParams = Utils::defaultSqlParams();
        sqlParams["iquan.plan.output.exec_params"] = string("true");
        sqlParams[IQUAN_PLAN_PREPARE_LEVEL] = string(IQUAN_JNI_POST_OPTIMIZE);
        sqlParams[IQUAN_PLAN_CACHE_ENALE] = string("false");

        {
            IquanDqlResponse response;
            PlanCacheStatus planCacheStatus;
            status = TestUtils::simpleQuery(pIquan,
                                            catalogName,
                                            databaseName,
                                            {sql},
                                            sqlParams,
                                            dynamicParams,
                                            response,
                                            planCacheStatus);
            ASSERT_TRUE(status.ok()) << status.errorMessage();

            ASSERT_FALSE(planCacheStatus.planGet);
            ASSERT_FALSE(planCacheStatus.planPut);
            ASSERT_EQ(0ul, pIquan->getPlanCacheKeyCount());
            ASSERT_EQ(0ul, pIquan->getPlanMetaCacheKeyCount());
        }

        sqlParams[IQUAN_PLAN_CACHE_ENALE] = string("true");
        {
            IquanDqlResponse response;
            PlanCacheStatus planCacheStatus;
            status = TestUtils::simpleQuery(pIquan,
                                            catalogName,
                                            databaseName,
                                            {sql},
                                            sqlParams,
                                            dynamicParams,
                                            response,
                                            planCacheStatus);
            ASSERT_TRUE(status.ok()) << status.errorMessage();

            ASSERT_FALSE(planCacheStatus.planGet);
            ASSERT_TRUE(planCacheStatus.planPut);
            ASSERT_FALSE(planCacheStatus.planMetaGet);
            ASSERT_TRUE(planCacheStatus.planMetaPut);
            ASSERT_EQ(1ul, pIquan->getPlanCacheKeyCount());
            ASSERT_EQ(1ul, pIquan->getPlanMetaCacheKeyCount());
        }
        {
            IquanDqlResponse response;
            PlanCacheStatus planCacheStatus;
            status = TestUtils::simpleQuery(pIquan,
                                            catalogName,
                                            databaseName,
                                            {sql},
                                            sqlParams,
                                            dynamicParams,
                                            response,
                                            planCacheStatus);
            ASSERT_TRUE(status.ok()) << status.errorMessage();

            ASSERT_TRUE(planCacheStatus.planGet);
            ASSERT_FALSE(planCacheStatus.planPut);
            ASSERT_TRUE(planCacheStatus.planMetaGet);
            ASSERT_FALSE(planCacheStatus.planMetaPut);
            ASSERT_EQ(1ul, pIquan->getPlanCacheKeyCount());
            ASSERT_EQ(1ul, pIquan->getPlanMetaCacheKeyCount());
        }
        {
            IquanDqlResponse response;
            vector<vector<autil::legacy::Any>> curDynamicParams;
            vector<autil::legacy::Any> curDynamicParam {-10,
                                                        10,
                                                        80,
                                                        string("hello"),
                                                        string("123456"),
                                                        string("-123456"),
                                                        string("whatareyoulookingat")};
            curDynamicParams.push_back(curDynamicParam);
            PlanCacheStatus planCacheStatus;
            status = TestUtils::simpleQuery(pIquan,
                                            catalogName,
                                            databaseName,
                                            {sql},
                                            sqlParams,
                                            curDynamicParams,
                                            response,
                                            planCacheStatus);
            ASSERT_TRUE(status.ok()) << status.errorMessage();

            ASSERT_TRUE(planCacheStatus.planGet);
            ASSERT_FALSE(planCacheStatus.planPut);
            ASSERT_TRUE(planCacheStatus.planMetaGet);
            ASSERT_FALSE(planCacheStatus.planMetaPut);
            ASSERT_EQ(1ul, pIquan->getPlanCacheKeyCount());
            ASSERT_EQ(1ul, pIquan->getPlanMetaCacheKeyCount());
        }
        {
            IquanDqlResponse response;
            vector<vector<autil::legacy::Any>> curDynamicParams;
            vector<autil::legacy::Any> curDynamicParam {39,
                                                        10,
                                                        90,
                                                        string("hello"),
                                                        string("123456"),
                                                        string("-123456"),
                                                        string("whatareyoulookingat")};
            curDynamicParams.push_back(curDynamicParam);
            PlanCacheStatus planCacheStatus;
            status = TestUtils::simpleQuery(pIquan,
                                            catalogName,
                                            databaseName,
                                            {sql},
                                            sqlParams,
                                            curDynamicParams,
                                            response,
                                            planCacheStatus);
            ASSERT_TRUE(status.ok()) << status.errorMessage();

            ASSERT_FALSE(planCacheStatus.planGet);
            ASSERT_TRUE(planCacheStatus.planPut);
            ASSERT_TRUE(planCacheStatus.planMetaGet);
            ASSERT_FALSE(planCacheStatus.planMetaPut);
            ASSERT_EQ(2ul, pIquan->getPlanCacheKeyCount());
            ASSERT_EQ(1ul, pIquan->getPlanMetaCacheKeyCount());
        }
        {
            IquanDqlResponse response;
            vector<vector<autil::legacy::Any>> curDynamicParams;
            vector<autil::legacy::Any> curDynamicParam {39,
                                                        10,
                                                        98,
                                                        string("hello"),
                                                        string("123456"),
                                                        string("-123456"),
                                                        string("whatareyoulookingat")};
            curDynamicParams.push_back(curDynamicParam);
            PlanCacheStatus planCacheStatus;
            status = TestUtils::simpleQuery(pIquan,
                                            catalogName,
                                            databaseName,
                                            {sql},
                                            sqlParams,
                                            curDynamicParams,
                                            response,
                                            planCacheStatus);
            ASSERT_TRUE(status.ok()) << status.errorMessage();

            ASSERT_TRUE(planCacheStatus.planGet);
            ASSERT_FALSE(planCacheStatus.planPut);
            ASSERT_TRUE(planCacheStatus.planMetaGet);
            ASSERT_FALSE(planCacheStatus.planMetaPut);
            ASSERT_EQ(2ul, pIquan->getPlanCacheKeyCount());
            ASSERT_EQ(1ul, pIquan->getPlanMetaCacheKeyCount());
        }
        {
            IquanDqlResponse response;
            vector<vector<autil::legacy::Any>> curDynamicParams;
            vector<autil::legacy::Any> curDynamicParam {39,
                                                        10,
                                                        80,
                                                        string("hello"),
                                                        string("123456"),
                                                        string("-123456"),
                                                        string("different")};
            curDynamicParams.push_back(curDynamicParam);
            PlanCacheStatus planCacheStatus;
            status = TestUtils::simpleQuery(pIquan,
                                            catalogName,
                                            databaseName,
                                            {sql},
                                            sqlParams,
                                            curDynamicParams,
                                            response,
                                            planCacheStatus);
            ASSERT_TRUE(status.ok()) << status.errorMessage();

            ASSERT_TRUE(planCacheStatus.planGet);
            ASSERT_FALSE(planCacheStatus.planPut);
            ASSERT_TRUE(planCacheStatus.planMetaGet);
            ASSERT_FALSE(planCacheStatus.planMetaPut);
            ASSERT_EQ(2ul, pIquan->getPlanCacheKeyCount());
            ASSERT_EQ(1ul, pIquan->getPlanMetaCacheKeyCount());
        }
        {
            IquanDqlResponse response;
            vector<vector<autil::legacy::Any>> curDynamicParams;
            vector<autil::legacy::Any> curDynamicParam {0,
                                                        10,
                                                        80,
                                                        string("hello"),
                                                        string("123456"),
                                                        string("123456"),
                                                        string("whatareyoulookingat")};
            curDynamicParams.push_back(curDynamicParam);
            PlanCacheStatus planCacheStatus;
            status = TestUtils::simpleQuery(pIquan,
                                            catalogName,
                                            databaseName,
                                            {sql},
                                            sqlParams,
                                            curDynamicParams,
                                            response,
                                            planCacheStatus);
            ASSERT_TRUE(status.ok()) << status.errorMessage();

            ASSERT_FALSE(planCacheStatus.planGet);
            ASSERT_TRUE(planCacheStatus.planPut);
            ASSERT_TRUE(planCacheStatus.planMetaGet);
            ASSERT_FALSE(planCacheStatus.planMetaPut);
            ASSERT_EQ(3ul, pIquan->getPlanCacheKeyCount());
            ASSERT_EQ(1ul, pIquan->getPlanMetaCacheKeyCount());
        }
        {
            IquanDqlResponse response;
            vector<vector<autil::legacy::Any>> curDynamicParams;
            vector<autil::legacy::Any> curDynamicParam {0,
                                                        11,
                                                        90,
                                                        string("world"),
                                                        string("123456"),
                                                        string("-123456"),
                                                        string("whatareyoulookingat")};
            curDynamicParams.push_back(curDynamicParam);
            PlanCacheStatus planCacheStatus;
            status = TestUtils::simpleQuery(pIquan,
                                            catalogName,
                                            databaseName,
                                            {sql},
                                            sqlParams,
                                            curDynamicParams,
                                            response,
                                            planCacheStatus);
            ASSERT_TRUE(status.ok()) << status.errorMessage();

            ASSERT_TRUE(planCacheStatus.planGet);
            ASSERT_FALSE(planCacheStatus.planPut);
            ASSERT_TRUE(planCacheStatus.planMetaGet);
            ASSERT_FALSE(planCacheStatus.planMetaPut);
            ASSERT_EQ(3ul, pIquan->getPlanCacheKeyCount());
            ASSERT_EQ(1ul, pIquan->getPlanMetaCacheKeyCount());
        }
        {
            IquanDqlResponse response;
            PlanCacheStatus planCacheStatus;
            status = TestUtils::simpleQuery(pIquan,
                                            catalogName,
                                            databaseName,
                                            {sql2},
                                            sqlParams,
                                            dynamicParams2,
                                            response,
                                            planCacheStatus);
            ASSERT_TRUE(status.ok()) << status.errorMessage();

            ASSERT_FALSE(planCacheStatus.planGet);
            ASSERT_TRUE(planCacheStatus.planPut);
            ASSERT_FALSE(planCacheStatus.planMetaGet);
            ASSERT_TRUE(planCacheStatus.planMetaPut);
            ASSERT_EQ(4ul, pIquan->getPlanCacheKeyCount());
            ASSERT_EQ(2ul, pIquan->getPlanMetaCacheKeyCount());
        }
        {
            IquanDqlResponse response;
            vector<vector<autil::legacy::Any>> curDynamicParams;
            vector<autil::legacy::Any> curDynamicParam {0};
            curDynamicParams.push_back(curDynamicParam);
            PlanCacheStatus planCacheStatus;
            status = TestUtils::simpleQuery(pIquan,
                                            catalogName,
                                            databaseName,
                                            {sql3},
                                            sqlParams,
                                            curDynamicParams,
                                            response,
                                            planCacheStatus);
            ASSERT_TRUE(status.ok()) << status.errorMessage();

            ASSERT_FALSE(planCacheStatus.planGet);
            ASSERT_TRUE(planCacheStatus.planPut);
            ASSERT_FALSE(planCacheStatus.planMetaGet);
            ASSERT_TRUE(planCacheStatus.planMetaPut);
            ASSERT_EQ(5ul, pIquan->getPlanCacheKeyCount());
            ASSERT_EQ(3ul, pIquan->getPlanMetaCacheKeyCount());
        }
        {
            IquanDqlResponse response;
            vector<vector<autil::legacy::Any>> curDynamicParams;
            vector<autil::legacy::Any> curDynamicParam {10};
            curDynamicParams.push_back(curDynamicParam);
            PlanCacheStatus planCacheStatus;
            status = TestUtils::simpleQuery(pIquan,
                                            catalogName,
                                            databaseName,
                                            {sql3},
                                            sqlParams,
                                            curDynamicParams,
                                            response,
                                            planCacheStatus);
            ASSERT_TRUE(status.ok()) << status.errorMessage();

            ASSERT_TRUE(planCacheStatus.planGet);
            ASSERT_FALSE(planCacheStatus.planPut);
            ASSERT_TRUE(planCacheStatus.planMetaGet);
            ASSERT_FALSE(planCacheStatus.planMetaPut);
            ASSERT_EQ(5ul, pIquan->getPlanCacheKeyCount());
            ASSERT_EQ(3ul, pIquan->getPlanMetaCacheKeyCount());
        }
        {
            IquanDqlResponse response;
            vector<vector<autil::legacy::Any>> curDynamicParams;
            PlanCacheStatus planCacheStatus;
            status = TestUtils::simpleQuery(pIquan,
                                            catalogName,
                                            databaseName,
                                            {sql4},
                                            sqlParams,
                                            curDynamicParams,
                                            response,
                                            planCacheStatus);
            ASSERT_TRUE(status.ok()) << status.errorMessage();

            ASSERT_FALSE(planCacheStatus.planGet);
            ASSERT_TRUE(planCacheStatus.planPut);
            ASSERT_FALSE(planCacheStatus.planMetaGet);
            ASSERT_TRUE(planCacheStatus.planMetaPut);
            ASSERT_EQ(6ul, pIquan->getPlanCacheKeyCount());
            ASSERT_EQ(4ul, pIquan->getPlanMetaCacheKeyCount());
        }
        {
            IquanDqlResponse response;
            vector<vector<autil::legacy::Any>> curDynamicParams;
            PlanCacheStatus planCacheStatus;
            status = TestUtils::simpleQuery(pIquan,
                                            catalogName,
                                            databaseName,
                                            {sql4},
                                            sqlParams,
                                            curDynamicParams,
                                            response,
                                            planCacheStatus);
            ASSERT_TRUE(status.ok()) << status.errorMessage();

            ASSERT_TRUE(planCacheStatus.planGet);
            ASSERT_FALSE(planCacheStatus.planPut);
            ASSERT_TRUE(planCacheStatus.planMetaGet);
            ASSERT_FALSE(planCacheStatus.planMetaPut);
            ASSERT_EQ(6ul, pIquan->getPlanCacheKeyCount());
            ASSERT_EQ(4ul, pIquan->getPlanMetaCacheKeyCount());
        }
        {
            IquanDqlResponse response;
            vector<vector<autil::legacy::Any>> curDynamicParams;
            pIquan->resetPlanCache();
            PlanCacheStatus planCacheStatus;
            status = TestUtils::simpleQuery(pIquan,
                                            catalogName,
                                            databaseName,
                                            {sql4},
                                            sqlParams,
                                            curDynamicParams,
                                            response,
                                            planCacheStatus);
            ASSERT_TRUE(status.ok()) << status.errorMessage();

            ASSERT_FALSE(planCacheStatus.planGet);
            ASSERT_TRUE(planCacheStatus.planPut);
            ASSERT_TRUE(planCacheStatus.planMetaGet);
            ASSERT_FALSE(planCacheStatus.planMetaPut);
            ASSERT_EQ(1ul, pIquan->getPlanCacheKeyCount());
            ASSERT_EQ(4ul, pIquan->getPlanMetaCacheKeyCount());
        }
        {
            IquanDqlResponse response;
            vector<vector<autil::legacy::Any>> curDynamicParams;
            pIquan->resetPlanCache();
            PlanCacheStatus planCacheStatus;
            status = TestUtils::simpleQuery(pIquan,
                                            catalogName,
                                            databaseName,
                                            {sql5},
                                            sqlParams,
                                            curDynamicParams,
                                            response,
                                            planCacheStatus);
            ASSERT_TRUE(status.ok()) << status.errorMessage();

            ASSERT_FALSE(planCacheStatus.planGet);
            ASSERT_TRUE(planCacheStatus.planPut);
            ASSERT_FALSE(planCacheStatus.planMetaGet);
            ASSERT_TRUE(planCacheStatus.planMetaPut);
            ASSERT_EQ(1ul, pIquan->getPlanCacheKeyCount());
            ASSERT_EQ(5ul, pIquan->getPlanMetaCacheKeyCount());
        }
    }
}

TEST_F(IquanTest, testReplaceParams) {
    string catalogRootPath = GET_PRIVATE_TEST_DATA_PATH_WITHIN_TEST() + string("/iquan_catalog");
    IquanPtr pIquan = TestUtils::createIquan();
    ASSERT_TRUE(pIquan != nullptr);

    vector<string> tableList = {string("t1.json"), string("t2.json")};
    Status status = TestUtils::registerTable(pIquan, catalogRootPath, tableList);
    ASSERT_TRUE(status.ok()) << status.errorMessage();
    vector<string> functionList = {string("udf.json"), string("udaf.json"), string("udtf.json")};
    status = TestUtils::registerFunction(pIquan, catalogRootPath, functionList);
    ASSERT_TRUE(status.ok()) << status.errorMessage();

    //
    string catalogName = "default";
    string databaseName = "db1";
    string sql = "SELECT i1, i2 FROM t1 WHERE _model_recall('pslr_dnn', 'qinfo') and i1 > 10";
    vector<vector<autil::legacy::Any>> dynamicParams;

    // for java cache
    {
        autil::legacy::json::JsonMap sqlParams = Utils::defaultSqlParams();
        sqlParams["iquan.plan.output.exec_params"] = string("true");
        sqlParams[IQUAN_PLAN_PREPARE_LEVEL] = string(IQUAN_REL_POST_OPTIMIZE);
        sqlParams[IQUAN_PLAN_CACHE_ENALE] = string("false");

        {
            IquanDqlResponse response;
            PlanCacheStatus planCacheStatus;
            status = TestUtils::simpleQuery(pIquan,
                                            catalogName,
                                            databaseName,
                                            {sql},
                                            sqlParams,
                                            dynamicParams,
                                            response,
                                            planCacheStatus);
            ASSERT_TRUE(status.ok()) << status.errorMessage();
            const auto &optimizeInfosMap = response.sqlPlan.optimizeInfosMap;
            auto iter = optimizeInfosMap.find("0");
            ASSERT_TRUE(iter != optimizeInfosMap.end());
            ASSERT_EQ(1, iter->second.size());
            const auto &optimizeInfos = iter->second[0];
            ASSERT_EQ(1, optimizeInfos.key.size());
            ASSERT_EQ("model_recall_0", optimizeInfos.key[0]);
            ASSERT_EQ(1, optimizeInfos.type.size());
            ASSERT_EQ("string", optimizeInfos.type[0]);
            ASSERT_EQ("_model_recall", optimizeInfos.optimizeInfo.op);
            ASSERT_EQ("UDF", optimizeInfos.optimizeInfo.type);
            ASSERT_EQ(2, optimizeInfos.optimizeInfo.params.size());
        }
    }
}

} // namespace iquan
