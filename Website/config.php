<?php 
define('DB_HOST'    , 'sql212.byethost12.com'); 
define('DB_USERNAME', 'b12_33156976'); 
define('DB_PASSWORD', '.8ZKp@D3X8xjHTe');    
define('DB_NAME'    , 'b12_33156976_cap312');

define('POST_DATA_URL', 'http://cap312.byethost12.com/sensordata.php');

//PROJECT_API_KEY is the exact duplicate of, PROJECT_API_KEY in NodeMCU sketch file
//Both values must be same
define('PROJECT_API_KEY', 'hello world');


//set time zone for your country
date_default_timezone_set('Africa/Cairo');

// Connect with the database 
$db = new mysqli(DB_HOST, DB_USERNAME, DB_PASSWORD, DB_NAME); 
 
// Display error if failed to connect 
if ($db->connect_errno) { 
    echo "Connection to database is failed: ".$db->connect_error;
    exit();
}
