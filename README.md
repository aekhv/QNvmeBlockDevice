# QNvmeBlockDevice

## Description
The QNvmeBlockDevice user application library allows to communicate with a NVMe device over the MiniNVMe kernel module. It is Qt based library written on C++ language.

## How to build
Just run Qt Creator and open the project file (.pro), then press Ctrl+B. As result a library file (.a) will be generated.

## How to use
I didn't wrote detailed documentarion because I'm a lazy ass. So take a look at the `mininvme-test-app` project, file `mainwindow.cpp` includes all the interesting things.

## PCIe Hot-Plug important notes for modern PC mainboards
To make PCIe Hot-Plug work some conditions must be fulfilled:
1. Use Linux kernel version 6.x.
2. You have to use system chipset that supports PCIe Hot-Plug capability. Type in terminal `sudo lspci -vv | grep HotPlug`. If you see `HotPlug+` in output it means some of the PCIe slots are support Hot-Plug. Of course you have to use these slots only.
3. At the boot time any NVMe device must be attached already, in any words a BIOS must to initialize PCIe bridge and attached NVMe device. You can use any working NVMe device as a kind of dummy. As soon as operating system is loaded you can remove dummy device and then attach faulty device. See `mininvme-test-app` for details.

![](/img/mininvme-test-app.png)
