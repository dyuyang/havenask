load('//bazel:defs.bzl', 'hdrs_pkg')
package_group(
    name='indexlibV1',
    packages=[
        '//aios/amalgam/...', '//aios/apps/...', '//aios/basic_ops/...',
        '//aios/coco_ops/...', '//aios/expression/...', '//aios/ha3/...',
        '//aios/hobbit_framework/...', '//aios/igraph/...',
        '//aios/plugins/...', '//aios/sql/...',
        '//aios/storage/indexlib/indexlib/...', '//aios/storage/kiwi/...',
        '//aios/storage/remote_indexlib/...', '//aios/suez/...',
        '//aios/suez_turing/...', '//aios/turbojet/...',
        '//aios/turing_ops_util/...', '//package/...', '//turing_plugins/...'
    ]
)
cc_library(
    name='indexlib_h',
    hdrs=['misc/common.h'],
    copts=['-Werror'],
    include_prefix='indexlib',
    visibility=[
        '//aios/storage/indexlib/indexlib:indexlibV1',
        '@com_taobao_turing_plugins//turing_plugins:__subpackages__'
    ],
    deps=[':index_define', ':indexlib_header', '//aios/autil:multi_value']
)
cc_library(
    name='indexlib_header',
    hdrs=['indexlib.h'],
    copts=['-Werror'],
    include_prefix='indexlib',
    visibility=['//aios/storage/indexlib/indexlib:__subpackages__'],
    deps=[
        '//aios/autil:multi_value', '//aios/storage/indexlib/legacy:indexlib'
    ]
)
cc_library(
    name='indexlib_misc',
    srcs=[],
    hdrs=[
        'common_define.h', 'misc/doc_tracer.h', 'misc/error_log_collector.h',
        'misc/log.h'
    ],
    copts=['-Werror'],
    include_prefix='indexlib',
    visibility=['//aios/storage/indexlib/indexlib:__subpackages__'],
    deps=[
        ':indexlib_h', '//aios/filesystem/fslib:fslib-framework',
        '//aios/kmonitor:kmonitor_client_cpp',
        '//aios/storage/indexlib/util:ErrorLogCollector',
        '//aios/storage/indexlib/util:PoolUtil'
    ],
    alwayslink=True
)
cc_library(
    name='index_define',
    hdrs=['index_define.h'],
    copts=['-Werror'],
    include_prefix='indexlib',
    visibility=['//aios/storage/indexlib/indexlib:__subpackages__'],
    deps=[
        '//aios/storage/indexlib/base:constants',
        '//aios/storage/indexlib/index/attribute:constants',
        '//aios/storage/indexlib/index/common:constants',
        '//aios/storage/indexlib/index/inverted_index:constants',
        '//aios/storage/indexlib/index/kkv:constants',
        '//aios/storage/indexlib/index/kv:constants',
        '//aios/storage/indexlib/index/primary_key:constants',
        '//aios/storage/indexlib/index/source:constants',
        '//aios/storage/indexlib/index/summary:constants'
    ]
)
cc_library(
    name='indexlib_plugin',
    srcs=glob(['plugin/*.cpp']),
    hdrs=glob(['plugin/*.h']),
    copts=['-Werror'],
    include_prefix='indexlib',
    visibility=['//visibility:public'],
    deps=[
        ':indexlib_h', ':indexlib_misc',
        '//aios/storage/indexlib/indexlib/config'
    ],
    alwayslink=True
)
hdrs_pkg(
    name='indexlib_codegen_header_package',
    srcs=[
        ':indexlib_headers_without_codegen_config',
        '//aios/storage/indexlib/base:Status',
        '//aios/storage/indexlib/base:Types',
        '//aios/storage/indexlib/config:IIndexConfig'
    ],
    prefix='usr/local/include',
    visibility=[':__subpackages__']
)
genrule(
    name='codegen_config_h',
    srcs=[
        'codegen/codegen_config.h.in', ':indexlib_codegen_header_package',
        ':codegen_package_tar'
    ],
    outs=['codegen/codegen_config.h'],
    cmd='\n    checksum=""\n    files="$(locations :indexlib_codegen_header_package) $(locations :codegen_package_tar)"\n    for f in $$files\n    do\n        checksum=$$checksum$$(md5sum $$f)\n    done\n    checksum=$$(md5sum <<< $$checksum | awk \'{print $$1}\')\n    sed s/version/$$checksum/ $(locations :codegen/codegen_config.h.in) > $(OUTS)\n    ',
    tags=['manual'],
    visibility=['//aios/storage/indexlib/indexlib:__subpackages__']
)
cc_library(
    name='indexlib_table',
    srcs=glob(['table/*.cpp']),
    hdrs=glob(['table/*.h']),
    copts=['-Werror'],
    include_prefix='indexlib',
    visibility=['//aios/storage/indexlib/indexlib:indexlibV1'],
    deps=[
        ':indexlib_h', ':indexlib_misc',
        '//aios/storage/indexlib/indexlib/common',
        '//aios/storage/indexlib/indexlib/config',
        '//aios/storage/indexlib/indexlib/document',
        '//aios/storage/indexlib/indexlib/index_base'
    ],
    alwayslink=True
)
cc_library(
    name='indexlib_merger',
    srcs=glob([
        'merger/*.cpp', 'merger/document_reclaimer/*.cpp',
        'merger/merge_strategy/*.cpp', 'merger/split_strategy/*.cpp'
    ]),
    hdrs=glob([
        'merger/*.h', 'merger/document_reclaimer/*.h',
        'merger/merge_strategy/*.h', 'merger/split_strategy/*.h'
    ]),
    copts=['-Werror'],
    include_prefix='indexlib',
    visibility=['//aios/storage/indexlib/indexlib:indexlibV1'],
    deps=[
        ':indexlib_h', ':indexlib_misc', ':indexlib_table',
        '//aios/storage/indexlib/index/inverted_index:AndPostingExecutor',
        '//aios/storage/indexlib/index/inverted_index:TermPostingExecutor',
        '//aios/storage/indexlib/indexlib/common',
        '//aios/storage/indexlib/indexlib/config',
        '//aios/storage/indexlib/indexlib/document:raw_document',
        '//aios/storage/indexlib/indexlib/index:calculator',
        '//aios/storage/indexlib/indexlib/index:index_external',
        '//aios/storage/indexlib/indexlib/index:segment_metrics_updater',
        '//aios/storage/indexlib/indexlib/index/normal/attribute:data_iterator'
    ],
    alwayslink=True
)
cc_library(
    name='third_party_lib',
    copts=['-Werror'],
    visibility=['//aios/storage/indexlib/indexlib:indexlibV1'],
    deps=(['@boost//:regex'] + []),
    alwayslink=True
)
cc_library(
    name='indexlib',
    copts=['-Werror'],
    visibility=['//aios/storage/indexlib/indexlib:indexlibV1'],
    deps=[
        ':third_party_lib',
        '//aios/storage/indexlib/indexlib/partition:indexlib_partition'
    ],
    alwayslink=True
)
cc_library(
    name='indexlib_testlib',
    copts=['-Werror'],
    include_prefix='indexlib',
    visibility=[
        '//aios/storage/indexlib/indexlib:indexlibV1',
        '@com_taobao_turing_plugins//turing_plugins:__subpackages__'
    ],
    deps=[
        ':indexlib_merger', '//aios/storage/indexlib/indexlib/config',
        '//aios/storage/indexlib/indexlib/document',
        '//aios/storage/indexlib/indexlib/test:partition_state_machine',
        '//aios/storage/indexlib/indexlib/testlib',
        '//aios/storage/indexlib/indexlib/testlib:fake-partition-reader',
        '//aios/storage/indexlib/indexlib/util:util_all'
    ],
    alwayslink=True
)
cc_library(
    name='indexlib_testbase_helper',
    copts=['-Werror'],
    include_prefix='indexlib',
    visibility=['//aios/storage/indexlib/indexlib:__subpackages__'],
    deps=['//aios/storage/indexlib/indexlib/test:testbase_helper']
)
cc_library(
    name='indexlib_testbase',
    srcs=[],
    copts=['-Werror'],
    include_prefix='indexlib',
    visibility=['//aios/storage/indexlib/indexlib:__subpackages__'],
    deps=[
        '//aios/storage/indexlib/indexlib/test:testbase_helper',
        '@com_google_protobuf//:protobuf'
    ],
    alwayslink=True
)
