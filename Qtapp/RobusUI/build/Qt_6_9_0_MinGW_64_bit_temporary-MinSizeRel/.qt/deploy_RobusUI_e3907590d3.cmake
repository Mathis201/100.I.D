include("C:/Uni/S1/Projet/Qtapp/RobusUI/build/Qt_6_9_0_MinGW_64_bit_temporary-MinSizeRel/.qt/QtDeploySupport.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/RobusUI-plugins.cmake" OPTIONAL)
set(__QT_DEPLOY_I18N_CATALOGS "qtbase;qtconnectivity")

qt6_deploy_runtime_dependencies(
    EXECUTABLE C:/Uni/S1/Projet/Qtapp/RobusUI/build/Qt_6_9_0_MinGW_64_bit_temporary-MinSizeRel/RobusUI.exe
    GENERATE_QT_CONF
)
