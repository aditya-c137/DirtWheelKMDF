/*++

Module Name:

    driver.c

Abstract:

    This file contains the driver entry points and callbacks.

Environment:

    Kernel-mode Driver Framework

--*/

// cd "C:/Program Files (x86)/Windows Kits/10/Debuggers/x64" && .\windbg.exe -k net:port=53000,key=3qs96wvwuocjl.2ondcfvpz50ic.3npukc4wbp6li.11anr5kfh66na
// dd nt!kd_default_mask L1
// ed kd_default_mask 0xf
// g

#include "driver.h"
#include "driver.tmh"

#ifdef ALLOC_PRAGMA
#pragma alloc_text (INIT, DriverEntry)
#pragma alloc_text (PAGE, KMDFDriver2EvtDeviceAdd)
#pragma alloc_text (PAGE, KMDFDriver2EvtDriverContextCleanup)
#pragma alloc_text (PAGE, KMDFDriver2EvtDriverUnload)
#endif

NTSTATUS
DriverEntry(
    _In_ PDRIVER_OBJECT  DriverObject,
    _In_ PUNICODE_STRING RegistryPath
    )
/*++

Routine Description:
    DriverEntry initializes the driver and is the first routine called by the
    system after the driver is loaded. DriverEntry specifies the other entry
    points in the function driver, such as EvtDevice and DriverUnload.

Parameters Description:

    DriverObject - represents the instance of the function driver that is loaded
    into memory. DriverEntry must initialize members of DriverObject before it
    returns to the caller. DriverObject is allocated by the system before the
    driver is loaded, and it is released by the system after the system unloads
    the function driver from memory.

    RegistryPath - represents the driver specific path in the Registry.
    The function driver can use the path to store driver related data between
    reboots. The path does not store hardware instance specific data.

Return Value:

    STATUS_SUCCESS if successful,
    STATUS_UNSUCCESSFUL otherwise.

--*/
{
    WDF_DRIVER_CONFIG config;
    NTSTATUS status = STATUS_SUCCESS;
    WDF_OBJECT_ATTRIBUTES attributes;
    UNREFERENCED_PARAMETER(RegistryPath);
    UNREFERENCED_PARAMETER(DriverObject);

    //
    // Initialize WPP Tracing
    //
    WPP_INIT_TRACING(DriverObject, RegistryPath);

    //TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_DRIVER, "%!FUNC! Entry");
    KdPrint(("Driver Entry\n"));

    //
    // Register a cleanup callback so that we can call WPP_CLEANUP when
    // the framework driver object is deleted during driver unload.
    //
    WDF_OBJECT_ATTRIBUTES_INIT(&attributes);
    attributes.EvtCleanupCallback = KMDFDriver2EvtDriverContextCleanup;
    

    //WDF_DRIVER_CONFIG_INIT(&config,
    //                       KMDFDriver2EvtDeviceAdd
    //                       );

    WDF_DRIVER_CONFIG_INIT(&config, NULL);

    config.DriverInitFlags = WdfDriverInitNonPnpDriver;

    config.EvtDriverUnload = KMDFDriver2EvtDriverUnload;

    //DriverObject->DriverUnload = KMDFDriver2EvtDriverUnload2;

    status = WdfDriverCreate(DriverObject,
                             RegistryPath,
                             //&attributes,
                             NULL,
                             &config,
                             WDF_NO_HANDLE
                             );

    if (!NT_SUCCESS(status)) {
        //TraceEvents(TRACE_LEVEL_ERROR, TRACE_DRIVER, "WdfDriverCreate failed %!STATUS!", status);
        KdPrint(("WdfDriverCreate failed %d\n",status));
        WPP_CLEANUP(DriverObject);
        return status;
    }

    //TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_DRIVER, "%!FUNC! Exit");
    KdPrint(("Function Exit\n"));

    return status;
}

NTSTATUS
KMDFDriver2EvtDeviceAdd(
    _In_    WDFDRIVER       Driver,
    _Inout_ PWDFDEVICE_INIT DeviceInit
    )
/*++
Routine Description:

    EvtDeviceAdd is called by the framework in response to AddDevice
    call from the PnP manager. We create and initialize a device object to
    represent a new instance of the device.

Arguments:

    Driver - Handle to a framework driver object created in DriverEntry

    DeviceInit - Pointer to a framework-allocated WDFDEVICE_INIT structure.

Return Value:

    NTSTATUS

--*/
{
    NTSTATUS status;

    UNREFERENCED_PARAMETER(Driver);

    PAGED_CODE();

    //TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_DRIVER, "%!FUNC! Entry");

    KdPrint(("EvtDeviceAdd\n"));

    status = KMDFDriver2CreateDevice(DeviceInit);

    KdPrint(("Create Device Status: %d",status));

    //TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_DRIVER, "%!FUNC! Exit");

    return status;
}

VOID
KMDFDriver2EvtDriverContextCleanup(
    _In_ WDFOBJECT DriverObject
    )
/*++
Routine Description:

    Free all the resources allocated in DriverEntry.

Arguments:

    DriverObject - handle to a WDF Driver object.

Return Value:

    VOID.

--*/
{
    UNREFERENCED_PARAMETER(DriverObject);

    PAGED_CODE();

    //TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_DRIVER, "%!FUNC! Entry");
    KdPrint(("Context Cleanup\n"));

    //
    // Stop WPP Tracing
    //
    WPP_CLEANUP(WdfDriverWdmGetDriverObject((WDFDRIVER)DriverObject));
}

VOID KMDFDriver2EvtDriverUnload(
    _In_ WDFDRIVER Driver
) {
    PDRIVER_OBJECT DriverObject;

    PAGED_CODE();

    KdPrint(("Driver Unload 1\n"));

    DriverObject = WdfDriverWdmGetDriverObject(Driver);

    WPP_CLEANUP(DriverObject);
}
//
//VOID KMDFDriver2EvtDriverUnload2(
//    _In_ PDRIVER_OBJECT DriverObject
//) {
//    UNREFERENCED_PARAMETER(DriverObject);
//
//    PAGED_CODE();
//
//    KdPrint(("Driver Unload 2\n"));
//}
