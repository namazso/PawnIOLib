// PawnIOLib - Library and tooling source to be used with PawnIO.
// Copyright (C) 2025  namazso <admin@namazso.eu>
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

#include <ntstatus.h>

#define WIN32_NO_STATUS

#include <windows.h>

#include <winternl.h>

#include "public/PawnIOLib.h"

#include "ioctl.h"

#undef RtlMoveMemory
EXTERN_C NTSYSAPI VOID NTAPI RtlMoveMemory(
  VOID UNALIGNED* Destination,
  CONST VOID UNALIGNED* Source,
  SIZE_T Length
);

PAWNIOAPI pawnio_version(PULONG version) {
  *version = 0x00020000; // 2.0.0
  return S_OK;
}

PAWNIONTAPI pawnio_version_nt(PULONG version) {
  *version = 0x00020000; // 2.0.0
  return STATUS_SUCCESS;
}

PAWNIOAPI pawnio_open(PHANDLE handle) {
  return HRESULT_FROM_WIN32(RtlNtStatusToDosError(pawnio_open_nt(handle)));
}

PAWNIONTAPI pawnio_open_nt(PHANDLE handle) {
  *handle = nullptr;
  UNICODE_STRING ustr{};
  RtlInitUnicodeString(&ustr, k_device_path);
  OBJECT_ATTRIBUTES attr{};
  attr.Length = sizeof(attr);
  attr.ObjectName = &ustr;
  IO_STATUS_BLOCK iosb{};
  return NtOpenFile(
    handle,
    GENERIC_READ | GENERIC_WRITE,
    &attr,
    &iosb,
    FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
    0
  );
}

PAWNIOAPI pawnio_load(HANDLE handle, const UCHAR* blob, SIZE_T size) {
  return HRESULT_FROM_WIN32(RtlNtStatusToDosError(pawnio_load_nt(handle, blob, size)));
}

PAWNIONTAPI pawnio_load_nt(HANDLE handle, const UCHAR* blob, SIZE_T size) {
  IO_STATUS_BLOCK iosb{};
  return NtDeviceIoControlFile(
    handle,
    nullptr,
    nullptr,
    nullptr,
    &iosb,
    IOCTL_PIO_LOAD_BINARY,
    (PVOID)blob,
    size,
    nullptr,
    0
  );
}

PAWNIOAPI pawnio_execute(
  HANDLE handle,
  PCSTR name,
  const ULONG64* in,
  SIZE_T in_size,
  PULONG64 out,
  SIZE_T out_size,
  PSIZE_T return_size
) {
  const auto status = pawnio_execute_nt(handle, name, in, in_size, out, out_size, return_size);
  return HRESULT_FROM_WIN32(RtlNtStatusToDosError(status));
}

PAWNIONTAPI pawnio_execute_nt(
  HANDLE handle,
  PCSTR name,
  const ULONG64* in,
  SIZE_T in_size,
  PULONG64 out,
  SIZE_T out_size,
  PSIZE_T return_size
) {
  *return_size = 0;
  static constexpr auto FN_NAME_LENGTH = 32u;
  const auto allocsize = in_size * sizeof(*in) + FN_NAME_LENGTH;
  const auto heap = GetProcessHeap();
  const auto p = (char*)HeapAlloc(heap, 0, allocsize);
  lstrcpynA(p, name, 31);
  p[31] = 0;
  if (in_size)
    RtlMoveMemory(p + 32, in, in_size * sizeof(*in));

  IO_STATUS_BLOCK iosb{};
  const auto status = NtDeviceIoControlFile(
    handle,
    nullptr,
    nullptr,
    nullptr,
    &iosb,
    IOCTL_PIO_EXECUTE_FN,
    p,
    allocsize,
    out,
    out_size * sizeof(*out)
  );
  if (NT_SUCCESS(status)) {
    *return_size = iosb.Information / sizeof(*out);
  }
  HeapFree(heap, 0, p);
  return status;
}

PAWNIOAPI pawnio_close(HANDLE handle) {
  return HRESULT_FROM_WIN32(RtlNtStatusToDosError(pawnio_close_nt(handle)));
}

PAWNIONTAPI pawnio_close_nt(HANDLE handle) {
  return NtClose(handle);
}

extern "C" BOOL WINAPI DllEntry(HINSTANCE, DWORD, LPVOID) {
  return TRUE;
}
