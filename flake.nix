{
  description = "CTF writeup generator";
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };
  outputs = {
    nixpkgs,
    flake-utils,
    ...
  }:
    flake-utils.lib.eachDefaultSystem (
      system: let
        pkgs = import nixpkgs {
          inherit system;
        };

        ctfnew-script = pkgs.writeShellScriptBin "ctfnew" ''
          if [ $# -ne 3 ]; then
            echo "Usage: ctfnew <event> <category> <challenge>"
            exit 1
          fi

          event="$1"
          category="$2"
          challenge="$3"
          writeup_dir="$PWD"
          new_dir="$writeup_dir/$event/$category/$challenge"

          if [ -d "$new_dir" ]; then
            echo "Error: Writeup already exists at $new_dir"
            exit 1
          fi

          mkdir -p "$new_dir" || {
            echo "Error: Could not create directory $new_dir"
            exit 1
          }

          if [ -f "$writeup_dir/template.md" ]; then
            sed -e "s/{{CHALLENGE_NAME}}/$challenge/g" \
                -e "s/{category}/$category/g" \
                "$writeup_dir/template.md" > "$new_dir/README.md"
            echo "Created writeup: $event/$category/$challenge"
            echo "Location: $new_dir"
          else
            echo "Warning: No template.md found in $writeup_dir"
            exit 1
          fi
        '';
      in {
        devShells.default = pkgs.mkShell {
          nativeBuildInputs = [
            pkgs.git-cliff
            pkgs.gnupg
            ctfnew-script
          ];
        };
      }
    );
}
