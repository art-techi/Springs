set(INCLUDE_LIST
    "${ASSIGNMENT_INCLUDE_ROOT}/DeBoor.hpp"
    "${ASSIGNMENT_INCLUDE_ROOT}/BSplineScene.hpp"
    "${ASSIGNMENT_INCLUDE_ROOT}/BSPline.hpp"
    "${ASSIGNMENT_INCLUDE_ROOT}/TensorPatchScene.hpp"
    "${ASSIGNMENT_INCLUDE_ROOT}/TensorPatch.hpp"
    "${ASSIGNMENT_INCLUDE_ROOT}/Node.hpp"
    "${ASSIGNMENT_INCLUDE_ROOT}/AngNode.hpp"
    "${ASSIGNMENT_INCLUDE_ROOT}/Scene.hpp"
    "${ASSIGNMENT_INCLUDE_ROOT}/AngScene.hpp"
    "${ASSIGNMENT_INCLUDE_ROOT}/Line.hpp"
    "${ASSIGNMENT_INCLUDE_ROOT}/AngLine.hpp"
    "${ASSIGNMENT_INCLUDE_ROOT}/torsionline.hpp"
    "${ASSIGNMENT_INCLUDE_ROOT}/torsionnode.hpp"
    "${ASSIGNMENT_INCLUDE_ROOT}/torsionscene.hpp"
    )

set(SHADER_INCLUDE "${ASSIGNMENT_INCLUDE_ROOT}/AssignmentPaths.hpp")
configure_file("${ASSIGNMENT_INCLUDE_ROOT}/AssignmentPaths.hpp.in"
    ${SHADER_INCLUDE})

set(ASSIGNMENT_INCLUDE_LIST
    ${INCLUDE_LIST}
    ${SHADER_INCLUDE}
    PARENT_SCOPE)
