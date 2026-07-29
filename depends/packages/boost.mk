package=boost
$(package)_version=1.77.0
$(package)_download_path=https://archives.boost.io/release/$($(package)_version)/source/
$(package)_download_path_fallback=https://github.com/QOGE/qogecoin/releases/download/depends-boost-1.77.0
$(package)_file_name=boost_$(subst .,_,$($(package)_version)).tar.bz2
$(package)_sha256_hash=fc9f85fc030e233142908241af7a846e60630aa7388de9a5fafb1f3a26840854

# Three-tier fetch, scoped to boost only: archives.boost.io (upstream Bitcoin
# Core's own fix for the now-dead boostorg.jfrog.io mirror — see
# bitcoin/bitcoin@ffbc173ca1) -> this project's GitHub-hosted mirror of the
# same sha256-verified tarball -> the project-wide FALLBACK_DOWNLOAD_PATH.
# Not implemented via FALLBACK_DOWNLOAD_PATH itself because the GitHub release
# tag (depends-boost-1.77.0) is package-and-version-specific: setting it
# globally would break the fallback for every other depends package the next
# time its primary source failed. Overriding $(package)_fetch_cmds per
# package is an existing pattern in this tree (see qt.mk).
define $(package)_fetch_cmds
  ( test -f $($(package)_source_dir)/$($(package)_file_name) || \
    ( $(call fetch_file_inner,$(package),$($(package)_download_path),$($(package)_file_name),$($(package)_file_name),$($(package)_sha256_hash)) || \
      $(call fetch_file_inner,$(package),$($(package)_download_path_fallback),$($(package)_file_name),$($(package)_file_name),$($(package)_sha256_hash)) || \
      $(call fetch_file_inner,$(package),$(FALLBACK_DOWNLOAD_PATH),$($(package)_file_name),$($(package)_file_name),$($(package)_sha256_hash))))
endef

define $(package)_stage_cmds
  mkdir -p $($(package)_staging_prefix_dir)/include && \
  cp -r boost $($(package)_staging_prefix_dir)/include
endef
