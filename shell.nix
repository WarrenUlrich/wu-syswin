with import <nixpkgs> {};

pkgs.mkShell rec {
  nativeBuildInputs = [ cmake pkg-config ];

  buildInputs = [
    vulkan-loader vulkan-headers
    xorg.libX11 xorg.libxcb
  ];
}
