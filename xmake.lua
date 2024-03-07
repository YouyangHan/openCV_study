
set_project("openCV_study")

add_rules("mode.debug", "mode.release")
set_config("arch", "x64")

-- for the windows platform (msvc)
if is_plat("windows") then 

    -- the release mode
    if is_mode("release") then
        add_cxflags("-MD") 
    -- the debug mode
    elseif is_mode("debug") then
        add_cxflags("-MDd") 
    end
end



add_packagedirs("./thirdLib")

target("openCV_study")
    add_rules("qt.application")
    --add_rules("qt.console")

    --头文件路径
    add_includedirs("./app")
    add_includedirs("./basis")
    
    --ui文件
    add_files("./app/Resources/*.ui")
    add_files("./basis/Resources/*.ui")

    -- metadata head files
    add_files("./app/*.h")
    add_files("./basis/*.h")

    -- qrc files
    add_files("./app/Resources/*.qrc")
    -- cpp files
    add_files("./app/*.cpp")
    add_files("./basis/*.cpp")


    --add_ldflags("/MANIFEST", "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\"", {force = true})
    

    add_packages("OpenCV4.6.0")
    add_frameworks("QtCore", "QtGui", "QtWidgets", "QtSql", "QtMultimedia", "QtMultimediaWidgets","QtNetwork","QtWinExtras")

