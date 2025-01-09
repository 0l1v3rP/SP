cmake_minimum_required(VERSION 3.22.1)
project(SP C)

set(CMAKE_C_STANDARD 11)
add_executable(SP src/main.c
        src/server/server.c
        src/server/comHandler.c
        src/server/business/userBusiness.c
        src/server/business/tableBusiness.c
        src/server/business/table2userBusiness.c
        src/server/data/tableData.c
        src/server/data/userData.c
        src/server/data/genericData.c
        src/server/data/table2userData.c
        src/server/data/sessionData.c
        src/client/client.c
        src/client/comHandler.c
        headers/config.h
        headers/server/server.h
        headers/server/comHandler.h
        headers/server/data/userData.h
        headers/server/data/tableData.h
        headers/server/data/table2userData.h
        headers/server/data/sessionData.h
        headers/server/data/genericData.h
        headers/server/business/table2userBusiness.h
        headers/server/business/userBusiness.h
        headers/server/business/tableBusiness.h
        headers/server/models/tableModel.h
        headers/server/models/table2userModel.h
        headers/server/models/sessionModel.h
        headers/server/models/userModel.h
        headers/client/comHandler.h
        headers/client/client.h
        headers/server/data/utilsData.h
        src/server/data/utilsData.c
)