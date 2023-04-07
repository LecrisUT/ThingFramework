%global upstream_name ThingFramework

Name:           thing-framework
Version:        0.0.0
Release:        %{autorelease}
Summary:        Extensible plugin system written for C++

License:        GPL-3.0-or-later
URL:            https://github.com/LecrisUT/%{upstream_name}
Source0:        https://github.com/LecrisUT/%{upstream_name}/archive/refs/tags/v%{version}.tar.gz
Source1:        %{name}.rpmlintrc

BuildArch:      noarch
BuildRequires:  cmake
BuildRequires:  gcc
BuildRequires:  gcc-c++
Requires:       cmake

%description
Extensible plugin system written for C++

%prep
%autosetup -n %{upstream_name}-%{version}

%build
%cmake
%cmake_build

%install
%cmake_install

%check
%ctest

%files
%license LICENSE.md
%doc README.md

%changelog
%autochangelog
