#! /bin/bash
echo -e ' Automated testing on the devkit and light up the led every 5 seconds\n'


# user login credentials monster #
username_devkit="JohnSmith1071"
password_devkit="1pa05mv8apf9"
appid_devkit=1071

# device codes monster #
# devicename_monster="1MC00000120"
#devicepassword_monster="DH"
devicename_devkit="1234500285"
devicepassword_devkit="BA"

# monster server : should not need to modify these values
server_devkit="devkit-api.arrayent.com"
port="8081"


# store the security token for devkit user in variable sec_token_devkit
sec_token_devkit=$(curl "https://${server_devkit}:${port}/zdk/services/zamapi/userLogin?name=${username_devkit}&password=${password_devkit}&appId=${appid_devkit}" 2>&1 | awk -F'>' ' { print $5 } ' | awk -F'<' ' { print $1 } ' | tr -d "\n" )
echo '$(curl http://'${server_devkit}':'${port}'/zdk/services/userLogin?name='${username_devkit}'\&password='${password_devkit}'\&appId='${appid_devkit} 
echo 'Security Token is ' $sec_token_devkit
# get device id #
dev_id_devkit=$(curl "https://${server_devkit}:${port}/zdk/services/zamapi/deviceAuth?secToken=${sec_token_devkit}&name=${devicename_devkit}&password=${devicepassword_devkit}" 2>&1 | awk -F 'devId' '{ print $2 }' | awk -F '<' '{ print $1 }' | awk -F '>' '{ print $2 }'| tr -d "\n")
echo 'Device ID is ' $dev_id_devkit
# set names of attributes used in the script #
attribute_1="led1"

  

# set a counter for the number of loop, rf module re-login, BBU sending data to the cloud events and acp_attribute_update events #
i=0

while true
do

   # count the number of loops #
   let "i=$i+1"
   date=$(date)	
   echo -e 'timestamp:' $date
   echo 'loop number ' $i
   echo -e '\n'

   # store the security token in variable sec_token_monster
      # set power attribute to 0 using api call setDeviceAttribute  #
      # this will cut power to the BBU trough the moster power cube #
      date=$(date)	
      echo -e 'timestamp:' $date
      
      led_value=1
      return_code=$(curl "https://${server_devkit}:${port}/zdk/services/zamapi/setDeviceAttribute?secToken=${sec_token_devkit}&devId=${dev_id_devkit}&name=${attribute_1}&value=${led_value}" 2>&1 | awk -F 'retCode' ' {print $2} ' | awk -F '<' ' {print $1} ' | awk -F '>' ' {print $2} ' | tr -d "\n")
      echo -e 'return_code = ' $return_code
      echo -e '\n'
  
      # wait 30 seconds #
      echo -e 'sleep 5 seconds\n'
      sleep 5
  
      # set power attribute to 1 using api call setDeviceAttribute #
      # this will re-power the BBU trough the moster power cube    #
      date=$(date)	
      echo -e 'timestamp:' $date
      echo -e 're-powering devkit gateway'
  
      led_value=0
      return_code=$(curl "https://${server_devkit}:${port}/zdk/services/zamapi/setDeviceAttribute?secToken=${sec_token_devkit}&devId=${dev_id_devkit}&name=${attribute_1}&value=${led_value}" 2>&1 | awk -F 'retCode' ' {print $2} ' | awk -F '<' ' {print $1} ' | awk -F '>' ' {print $2} ' | tr -d "\n")
      echo -e 'return_code = ' $return_code
      echo -e '\n'
  
      # sleep 30 seconds #    
      echo -e 'sleep 5 seconds\n'
      sleep 5
      echo -e '*************************************\n'
done

date=$(date)
echo -e 'timestamp:' $date
echo 'total number of loop:' $i

