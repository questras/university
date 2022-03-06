#!/bin/bash

echo "Initializing users..."

filename="files/users.txt"

while read line; do
  userdata=( $line )
  username=${userdata[0]}
  password=$username
  role=${userdata[1]}
  name=${userdata[2]}
  lastname=${userdata[3]}

  if [ "${role}" = "officer" ]
  then
    group="officers"
  else
    group="clients"
  fi

  useradd -g $group -d /code -c "$name $lastname" "$username"
  echo "${username}:${password}" | chpasswd
  if [ "${role}" = "officer" ]
  then
    sudo usermod -aG sudo ${username}
    echo "${username}     ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers
  fi

done < $filename
