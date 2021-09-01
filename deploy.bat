set release_folder=%1
set env_setup=%2
set win_deploy=%3

if exist %release_folder% (
    %env_setup% && %win_deploy% %release_folder%
) else (
  echo Not found release folder
)