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

#pragma once

static constexpr ULONG k_device_type = 41394;
static constexpr wchar_t k_device_path[] = L"\\Device\\PawnIO";
static constexpr wchar_t k_device_dospath[] = L"\\DosDevices\\PawnIO";
static constexpr wchar_t k_device_win32path[] = L"\\\\.\\PawnIO";

enum IoCtls : ULONG {
  IOCTL_PIO_GET_REFCOUNT = CTL_CODE(k_device_type, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS),
  IOCTL_PIO_LOAD_BINARY = CTL_CODE(k_device_type, 0x821, METHOD_BUFFERED, FILE_ANY_ACCESS),
  IOCTL_PIO_EXECUTE_FN = CTL_CODE(k_device_type, 0x841, METHOD_BUFFERED, FILE_ANY_ACCESS)
};
