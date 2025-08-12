QT       += core gui sql network charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
RC_FILE =  $$PWD/resource/app.rc
CONFIG += c++11


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    announce1.cpp \
    main.cpp \
    ceil_main.cpp \
    login.cpp \
    property_manager.cpp \
    proper_manageradd.cpp \
    property_staffadd.cpp \
    property_staffdel.cpp \
    property_managerdel.cpp \
    owner_evaluation.cpp \
    owner_pay.cpp \
    owner1(2).cpp \
    parking_apply.cpp \
    add_carfield_information.cpp \
    add_carfield_rental.cpp \
    add_need_to_pay.cpp \
    property_staff.cpp \
    update_carfield_information.cpp \
    update_carfield_rental_information.cpp \
    add_owner_information.cpp \
    function.cpp \
    vehicle_recognition.cpp

# 头文件列表（修正命名）
HEADERS += \
    CenteredDelegate1.h \
    announce1.h \
    ceil_main.h \
    login.h \
    property_manager.h \
    proper_manageradd.h \
    property_staffadd.h \
    property_staffdel.h \
    property_managerdel.h \
    owner_evaluation.h \
    owner_pay.h \
    owner1(2).h \
    parking_apply.h \
    centereddelegate.h \
    add_carfield_information.h \
    add_carfield_rental.h \
    add_need_to_pay.h \
    property_staff.h \
    update_carfield_information.h \
    update_carfield_rental_information.h \
    add_owner_information.h \
    function.h \
    vehicle_recognition.h

# UI文件列表（修正命名）
FORMS += \
    announce1.ui \
    ceil_main.ui \
    login.ui \
    property_manager.ui \
    proper_manageradd.ui \
    property_staffadd.ui \
    property_staffdel.ui \
    property_managerdel.ui \
    owner_evaluation.ui \
    owner_pay.ui \
    owner1(2).ui \
    parking_apply.ui \
    add_carfield_information.ui \
    add_carfield_rental.ui \
    add_need_to_pay.ui \
    property_staff.ui \
    update_carfield_information.ui \
    update_carfield_rental_information.ui \
    add_owner_information.ui \
    vehicle_recognition.ui
TRANSLATIONS += \
    IntelligentCommunity_za_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource/picture.qrc
