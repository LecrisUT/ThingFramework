Name:           thing-framework
Version:        0.0.0
Release:        %{autorelease}
Summary:        Extensible plugin system written for C++

License:        GPL-3.0-or-later
URL:            https://github.com/LecrisUT/ThingFramework
Source:         https://github.com/LecrisUT/ThingFramework/archive/refs/tags/v%{version}.tar.gz

BuildArch:      noarch
BuildRequires:  cmake
BuildRequires:  ninja-build
BuildRequires:  clang
BuildRequires:  clang-tools-extra
BuildRequires:  cmake-extra-utils
BuildRequires:  catch-devel
BuildRequires:  json-devel
Requires:       cmake

%description
Extensible plugin system written for C++

%package devel
Summary:   Development package for thing-framework
Requires:  thing-framework%{?_isa} = %{version}-%{release}
Requires:  cmake-extra-utils

%description devel
This package contains the development files for ThingFramework library


%prep
%autosetup -n ThingFramework-%{version}


%build
%global toolchain clang
%cmake \
    -GNinja \
    -DTHINGFRAMEWORK_TEST=ON
%cmake_build


%install
%cmake_install


%check
%ctest


%files
%{_libdir}/libThingFramework.so.*
%license LICENSE.md
%doc README.md

%files devel
%{_libdir}/libThingFramework.so
%{_libdir}/cmake/ThingFramework

%changelog
%autochangelog
