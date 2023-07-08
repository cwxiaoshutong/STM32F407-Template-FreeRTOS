#!/usr/bin/env bash

# @Author:          cwxiaos
# @Date:            2023-04-10 20:01:13
# @LastEditTime:    2023-04-16 20:43:21
# @Brief:           连续编译、烧录、打开串口监视器

# 决定是否自动打开串口监视器
monitor=true

make -j 32
if [ $? -ne 0 ];then
    echo -e "\e[31mBuild Failed \e[0m"
    exit 1
else
    echo -e "\e[32mBuild Success, Flashing \e[0m"
fi

make flash
if [ $? -ne 0 ];then
    echo -e "\e[31mFlash Failed \e[0m"
    exit 1
else
    echo -e "\e[32mFlash Success \e[0m"
fi

if [ "$monitor" = true ] ; then
    echo -e "\e[32mOpening Monitor \e[0m"
    make monitor
else
    read -p "Open Monitor? [Y/N]" -n 1 -r
    if [[ $REPLY =~ ^[Yy]$ ]];then
        echo -e "\e[32mOpening Monitor \e[0m"
        make monitor
    else
        echo -e "\e[32mAll Done, Exit 0 \e[0m"
    fi
fi