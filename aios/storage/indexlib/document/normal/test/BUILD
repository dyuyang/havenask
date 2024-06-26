load('//aios/storage:defs.bzl', 'strict_cc_fast_test', 'strict_cc_library')
strict_cc_library(
    name='normal_parser_test_helper',
    testonly=True,
    srcs=['SimpleTokenizer.cpp', 'TokenizeHelper.cpp'],
    hdrs=['SimpleTokenizer.h', 'TokenizeHelper.h'],
    copts=['-fno-access-control'],
    deps=[
        '//aios/storage/indexlib/config:TabletSchema',
        '//aios/storage/indexlib/document/normal:NormalExtendDocument',
        '//aios/storage/indexlib/document/normal/tokenize:AnalyzerToken',
        '//aios/storage/indexlib/document/normal/tokenize:TokenizeDocument',
        '//aios/storage/indexlib/index/primary_key:constants'
    ]
)
strict_cc_fast_test(
    name='normal_document_parser_test',
    srcs=['NormalDocumentParserTest.cpp', 'NullFieldAppenderTest.cpp'],
    copts=['-fno-access-control'],
    data=[
        'testdata/document_parser_simple_schema.json',
        'testdata/schema/document_parser_simple_schema.json',
        'testdata/schema_summary_reuse_source.json'
    ],
    deps=[
        ':normal_parser_test_helper',
        '//aios/storage/indexlib/document/normal:NormalDocumentParser',
        '//aios/storage/indexlib/document/normal:NullFieldAppender',
        '//aios/storage/indexlib/document/raw_document:DefaultRawDocument',
        '//aios/storage/indexlib/document/raw_document/test:RawDocumentMaker',
        '//aios/storage/indexlib/framework:tablet_schema_loader',
        '//aios/storage/indexlib/table/normal_table:table',
        '//aios/storage/indexlib/table/normal_table/test:NormalTabletSchemaMaker',
        '//aios/unittest_framework'
    ]
)
strict_cc_fast_test(
    name='source_document_test',
    srcs=['SourceDocumentTest.cpp'],
    copts=['-fno-access-control'],
    data=[],
    deps=[
        '//aios/storage/indexlib/document/normal:SourceDocument',
        '//aios/storage/indexlib/document/raw_document:DefaultRawDocument',
        '//aios/unittest_framework'
    ]
)
strict_cc_fast_test(
    name='source_formatter_test',
    srcs=['SourceFormatterTest.cpp'],
    copts=['-fno-access-control'],
    data=[],
    deps=[
        '//aios/storage/indexlib/config:IndexConfigDeserializeResource',
        '//aios/storage/indexlib/document/normal:SourceFormatter',
        '//aios/storage/indexlib/document/raw_document:DefaultRawDocument',
        '//aios/unittest_framework'
    ]
)
strict_cc_fast_test(
    name='ClassifiedDocumentTest',
    srcs=['ClassifiedDocumentTest.cpp'],
    copts=['-fno-access-control'],
    deps=[
        '//aios/storage/indexlib/document/normal:ClassifiedDocument',
        '//aios/storage/indexlib/document/raw_document:DefaultRawDocument',
        '//aios/unittest_framework'
    ]
)
strict_cc_fast_test(
    name='normal_document_test',
    srcs=['NormalDocumentTest.cpp'],
    copts=['-fno-access-control'],
    data=[],
    deps=[
        '//aios/storage/indexlib/document/normal:NormalDocument',
        '//aios/unittest_framework'
    ]
)
strict_cc_fast_test(
    name='single_document_parser_test',
    srcs=['SingleDocumentParserTest.cpp'],
    copts=['-fno-access-control'],
    data=['testdata/schema_summary_reuse_source.json'],
    deps=[
        '//aios/storage/indexlib/document/raw_document:DefaultRawDocument',
        '//aios/storage/indexlib/framework:tablet_schema_loader',
        '//aios/storage/indexlib/table/normal_table:table',
        '//aios/unittest_framework'
    ]
)
