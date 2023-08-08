<?php
  // phpinfo();


  class DeviceData {
    public $project;
    public $version;
    public $deviceID;
    public $ipAddress;
    public $time;
  }

  // get input data from POST-Request
  $input = (array) json_decode(file_get_contents('php://input'), TRUE);

  // fill input Data in DeviceData
  $device = new DeviceData();
  if (isset($input['Project'])) {
    $device->project = $input['Project'];
  }
  if (isset($input['Version'])) {
    $device->version = $input['Version'];
  }
  if (isset($input['DeviceID'])) {
    $device->deviceID = $input['DeviceID'];
  }
  if (isset($input['IPAddress'])) {
    $device->ipAddress = $input['IPAddress'];
  }
  if (isset($input['Time'])) {
    $device->time = $input['Time'];
  }
  var_dump($device);

  // convert DateTime
  $fixed = date('d.m.Y H:i:s', strtotime($device->time));
  echo $fixed, PHP_EOL;


/*
  // iterate through all objects
  foreach ($input as $key => $value) {
    echo "$key => $value\n";
    //var_dump($key);

    if (is_array($value)) {
      foreach($value as $i => $item) {
        //echo $value[$i], PHP_EOL;   // $key[$i] is same as $item
        echo $item, PHP_EOL;
      }   
    }
  }
  */


?>