set(SOURCE_LIST
    "${ASSIGNMENT_SOURCE_ROOT}/main.cpp"
    "${ASSIGNMENT_SOURCE_ROOT}/BSplineScene.cpp"
    "${ASSIGNMENT_SOURCE_ROOT}/BSpline.cpp"
    "${ASSIGNMENT_SOURCE_ROOT}/TensorPatchScene.cpp"
    "${ASSIGNMENT_SOURCE_ROOT}/TensorPatch.cpp"
    "${ASSIGNMENT_SOURCE_ROOT}/Node.cpp"
    "${ASSIGNMENT_SOURCE_ROOT}/AngNode.cpp"
    "${ASSIGNMENT_SOURCE_ROOT}/Scene.cpp"
    "${ASSIGNMENT_SOURCE_ROOT}/AngScene.cpp"
    "${ASSIGNMENT_SOURCE_ROOT}/Line.cpp"
    "${ASSIGNMENT_SOURCE_ROOT}/AngLine.cpp"
    "${ASSIGNMENT_SOURCE_ROOT}/torsionline.cpp"
    "${ASSIGNMENT_SOURCE_ROOT}/torsionnode.cpp"
    "${ASSIGNMENT_SOURCE_ROOT}/torsionscene.cpp"
    )

set(SHADER_SOURCE "${ASSIGNMENT_SOURCE_ROOT}/AssignmentPaths.cpp")
configure_file("${ASSIGNMENT_SOURCE_ROOT}/AssignmentPaths.cpp.in"
    ${SHADER_SOURCE})

set(ASSIGNMENT_SOURCE_LIST
    ${SOURCE_LIST}
    ${SHADER_SOURCE}
    PARENT_SCOPE)
