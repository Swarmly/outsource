function exit_handler() {
  echo ""
  echo ""
  read -n 1 -s -r -p "$1"
  exit
}

$1 "$2" "$3" "$4" || exit_handler "Build failed! Press any key to exit"

exit_handler "Build succeeded! Press any key to exit"