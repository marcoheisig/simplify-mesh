# rudimentary detection of VCG
# The following variables will be defined:
#  VCG_FOUND
#  VCG_INCLUDE_DIR
#  VCG_LIBRARIES

find_path(VCG_INCLUDE_DIR vcg/math/base.h)
list(APPEND _VCG_REQUIRED_VARS VCG_INCLUDE_DIR)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(VCG REQUIRED_VARS ${_VCG_REQUIRED_VARS})
unset(_VCG_REQUIRED_VARS)
