<?php

        $dbhost = 'db5007242110.hosting-data.io';
        $dbuser = 'dbu1168746';
        $dbpass = 'qwe159ZXC';
           
        $database = "dbs5969492";
           
        $pdo = new PDO("mysql:host=$dbhost;dbname=$database", $dbuser, $dbpass);

        $resuls = $pdo->query("SELECT id, Exterior, Medio, PuertaA, PuertaB FROM Puertas WHERE id=1 LIMIT 1")->fetch();

        $Exterior= $resuls['Exterior'];
        $Medio= $resuls['Medio'];
        $PuertaA= $resuls['PuertaA'];
        $PuertaB= $resuls['PuertaB'];  

  echo $Exterior . "," . $Medio . "," . $PuertaA . "," . $PuertaB; 

      $sql = "UPDATE Puertas SET Exterior=0,Medio=0,PuertaA=0,PuertaB=0 WHERE id=1";

      $stmt= $pdo->prepare($sql);
      $stmt->execute();
      
      $publisher_id = $pdo->lastInsertId();

      mysqli_close($conn); 

?>