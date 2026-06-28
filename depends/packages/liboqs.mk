package=liboqs
$(package)_version=0.15.0
$(package)_download_path=https://github.com/open-quantum-safe/liboqs/archive/refs/tags
$(package)_download_file=$($(package)_version).tar.gz
$(package)_file_name=liboqs-$($(package)_version).tar.gz
$(package)_sha256_hash=3983f7cd1247f37fb76a040e6fd684894d44a84cecdcfbdb90559b3216684b5c

define $(package)_config_cmds
  $($(package)_cmake) \
    -DCMAKE_SYSTEM_PROCESSOR=$(host_arch) \
    -DBUILD_SHARED_LIBS=OFF \
    -DBUILD_TESTING=OFF \
    -DOQS_USE_OPENSSL=ON \
    -DOQS_DIST_BUILD=ON \
    -DCMAKE_BUILD_TYPE=Release \
    .
endef

define $(package)_build_cmds
  $(MAKE)
endef

define $(package)_stage_cmds
  $(MAKE) DESTDIR=$($(package)_staging_dir) install
endef

define $(package)_postprocess_cmds
  rm -rf bin share
endef
