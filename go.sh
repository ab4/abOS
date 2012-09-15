#!/bin/bash
cd src;
make;
if [ $? -eq 0 ]
 then
  cd ..;
  ./update_image.sh;
  ./run_bochs.sh
 else
  echo "make failed"
fi


