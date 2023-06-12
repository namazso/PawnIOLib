// PawnIOLib - Library and tooling source to be used with PawnIO.
// Copyright (C) 2023  namazso <admin@namazso.eu>
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

#include <windows.h>

#include <winternl.h>

#include "public/PawnIOLib.h"

#include "ioctl.h"

#undef RtlCopyMemory
extern "C" __declspec(dllimport) void RtlCopyMemory(
  void* Destination,
  const void* Source,
  size_t Length
);

PAWNIOAPI pawnio_open(PHANDLE handle) {
  *handle = nullptr;
  UNICODE_STRING ustr{};
  RtlInitUnicodeString(&ustr, k_device_path);
  OBJECT_ATTRIBUTES attr{};
  attr.Length = sizeof(attr);
  attr.ObjectName = &ustr;
  HANDLE h{};
  IO_STATUS_BLOCK iosb{};
  const auto status = NtOpenFile(
    &h,
    GENERIC_READ | GENERIC_WRITE,
    &attr,
    &iosb,
    FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
    0
  );
  *handle = h;
  return HRESULT_FROM_NT(status);
}

PAWNIOAPI pawnio_load(HANDLE handle, const UCHAR* blob, SIZE_T size) {
  IO_STATUS_BLOCK iosb{};
  const auto status = NtDeviceIoControlFile(
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
  return HRESULT_FROM_NT(status);
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
  *return_size = 0;
  static constexpr auto FN_NAME_LENGTH = 32u;
  const auto allocsize = in_size * sizeof(*in) + FN_NAME_LENGTH;
  const auto heap = GetProcessHeap();
  const auto p = (char*)HeapAlloc(heap, 0, allocsize);
  lstrcpynA(p, name, 31);
  p[31] = 0;
  if (in_size)
    RtlCopyMemory(p + 32, in, in_size * sizeof(*in));

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
  return HRESULT_FROM_NT(status);
}

PAWNIOAPI pawnio_close(HANDLE handle) {
  return HRESULT_FROM_NT(NtClose(handle));
}

extern "C" BOOL WINAPI DllEntry(HINSTANCE, DWORD, LPVOID) {
  return TRUE;
}
