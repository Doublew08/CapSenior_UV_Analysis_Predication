<script type="module">
  // Import the functions you need from the SDKs you need
  import { initializeApp  } from "https://www.gstatic.com/firebasejs/9.15.0/firebase-app.js";
  import { getAnalytics } from "https://www.gstatic.com/firebasejs/9.15.0/firebase-analytics.js";
  import {getDatabase, set, get, update, remove, ref, child} from "https://www.gstatic.com/firebasejs/9.15.0/firebase-database.js";
  // TODO: Add SDKs for Firebase products that you want to use
  // https://firebase.google.com/docs/web/setup#available-libraries

  // Your web app's Firebase configuration
  // For Firebase JS SDK v7.20.0 and later, measurementId is optional
  const firebaseConfig = {
    apiKey: "AIzaSyDh97MwB8KWgPNi28444v48DQvjE9pZLm8",
    authDomain: "cap312-ef948.firebaseapp.com",
    databaseURL: "https://cap312-ef948-default-rtdb.europe-west1.firebasedatabase.app",
    projectId: "cap312-ef948",
    storageBucket: "cap312-ef948.appspot.com",
    messagingSenderId: "800156505950",
    appId: "1:800156505950:web:799c3221ef2493a49be1cf",
    measurementId: "G-26MS7GZ3PB"
  };

  // Initialize Firebase
  const app = initializeApp(firebaseConfig);
  const dbRef = ref(getDatabase());
  
  
  function refreshData()
  {
get(dbRef).then((snapshot) => {
  if (snapshot.exists()) {
  var dataa = snapshot.val()["UsersData"]["CIcQtf1r7DSkpwNDjOqEkh42Ire2"]["readings"];
  
  let count = 1;
  for(let x in dataa)
  {
    if(count < Object.keys(dataa).length)
    {
      count += 1;
    }
    else
    {
        console.log(dataa[x]);
        var name = Object.values(dataa[x]);
        document.cookie = "data=" + name;
        count = 0;
    }
  }
    
  } else {
    console.log("No data available");
  }
}).catch((error) => {
  console.error(error);
});
  }


setInterval(refreshData, 1000);
</script>

<?php
  $test = $_COOKIE['data'];
  //echo $test;
  include_once('esp-database.php');
  // Keep this API Key value to be compatible with the ESP code provided in the project page. If you change this value, the ESP sketch needs to match
  $api_key_value = "hellow world";
  $api_key= $sensor = $location = $value1 = $value2 = $value3 = "";
  $pieces = explode("&", $test);
  //echo (ltrim(stristr($pieces[0], '='), '='));
  if (true) {
    $api_key = test_input(ltrim(stristr($pieces[0], '='), '='));
    if($api_key == $api_key_value) {
      $sensor = test_input(ltrim(stristr($pieces[1], '='), '='));
      $location = test_input(ltrim(stristr($pieces[2], '='), '='));
      $value1 = test_input(ltrim(stristr($pieces[3], '='), '='));
      $value2 = test_input(ltrim(stristr($pieces[4], '='), '='));
      $value3 = test_input(ltrim(stristr($pieces[5], '='), '='));

      $result = insertReading($sensor, $location, $value1, $value2, $value3);
      //echo $result;
    }
    else {
      echo "Wrong API Key provided.";
      echo $test;
    }
  }
  else {
    echo "No data posted with HTTP POST.";
  }
  function test_input($data) {
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
  }
?>