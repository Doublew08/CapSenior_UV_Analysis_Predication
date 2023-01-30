<?php
   use Kreait\Firebase\Factory;
   use Kreait\Firebase\ServiceAccount;

   // This assumes that you have placed the Firebase credentials in the same directory
   // as this PHP file.
   $serviceAccount = ServiceAccount::fromJsonFile(__DIR__ . '/cap312-ef948-firebase-adminsdk-i2qji-0518437284.json');
   $firebase = (new Factory)
      ->withServiceAccount($serviceAccount)
      ->withDatabaseUri('https://cap312-ef948-default-rtdb.europe-west1.firebasedatabase.app/')
      ->create();
      
   $database = $firebase->getDatabase();
?>