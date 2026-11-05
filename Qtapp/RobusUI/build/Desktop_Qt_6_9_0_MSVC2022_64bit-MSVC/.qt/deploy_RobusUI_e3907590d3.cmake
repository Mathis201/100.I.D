include("C:/Uni/S1/Projet/Qtapp/RobusUI/build/Desktop_Qt_6_9_0_MSVC2022_64bit-MSVC/.qt/QtDeploySupport.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/RobusUI-plugins.cmake" OPTIONAL)
set(__QT_DEPLOY_I18N_CATALOGS "qtbase;qtconnectivity")

qt6_deploy_runtime_dependencies(
    EXECUTABLE C:/Uni/S1/Projet/Qtapp/RobusUI/build/Desktop_Qt_6_9_0_MSVC2022_64bit-MSVC/RobusUI.exe
    GENERATE_QT_CONF
)
