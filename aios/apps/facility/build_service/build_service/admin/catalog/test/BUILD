load('@rules_cc//cc:defs.bzl', 'cc_library', 'cc_test')
package(
    default_visibility=[
        '//aios/apps/facility/build_service/build_service/admin/catalog/test:__subpackages__'
    ]
)
cc_library(
    name='test_helper',
    testonly=True,
    srcs=glob(['Fake*.cpp']),
    hdrs=glob(['Fake*.h']),
    include_prefix='build_service',
    deps=['//aios/apps/facility/build_service:bs_admin']
)
cc_test(
    name='catalog_test',
    srcs=glob(['*Test.cpp']),
    copts=['-fno-access-control'],
    deps=[
        ':test_helper',
        '//aios/apps/facility/build_service:bs_proto_test_helper',
        '//aios/apps/facility/build_service:bs_testbase'
    ]
)
