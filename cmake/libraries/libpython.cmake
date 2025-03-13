# [SEE] "https://cmake.org/cmake/help/latest/module/FindPython.html"
find_package(
    Python 
    REQUIRED 
    COMPONENTS 
    Interpreter Development.Module Development.Embed Development.SABIModule
)