<?php

  if (isset($_GET['lampId']))
  {
        $dbhost = 'db5007242110.hosting-data.io';
        $dbuser = 'dbu1168746';
        $dbpass = 'qwe159ZXC';
           
        $database = "dbs5969492";
           
        $pdo = new PDO("mysql:host=$dbhost;dbname=$database", $dbuser, $dbpass);


        $lampId = $_GET['lampId'];

        $resuls = $pdo->query("SELECT id, data FROM EstadosLampara where lampId = $lampId LIMIT 1")->fetch();

        $id= $resuls['id'];
        $data= $resuls['data']; 

  echo $data ; 


      $sql = "UPDATE EstadosLampara SET data='' WHERE lampId = $lampId";

      $stmt= $pdo->prepare($sql);
      $stmt->execute();
      
      $publisher_id = $pdo->lastInsertId();

      mysqli_close($conn); 
  }
?>