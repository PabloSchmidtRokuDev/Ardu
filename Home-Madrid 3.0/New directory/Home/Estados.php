<?php
    $dbhost = 'db5007242110.hosting-data.io';
    $dbuser = 'dbu1168746';
    $dbpass = 'qwe159ZXC';
         
    $database = "dbs5969492";
         
    $pdo = new PDO("mysql:host=$dbhost;dbname=$database", $dbuser, $dbpass);


    if (isset($_GET['puerta']) && isset($_GET['estado']))
    {

        $puerta = $_GET['puerta'];
        $estado = $_GET['estado'];

        $sql = "";
        if ($puerta == "Exterior"){
            $sql = "UPDATE Puertas SET Exterior=0 WHERE id=1";

        }else if ($puerta == "Medio"){
            $sql = "UPDATE Puertas SET Medio=0 WHERE id=1";

        }else if ($puerta == "PuertaA"){
            $sql = "UPDATE Puertas SET PuertaA=0 WHERE id=1";

        }else if ($puerta == "PuertaB"){
            $sql = "UPDATE Puertas SET PuertaB=0 WHERE id=1";

        } 

        if ( $sql != "") {

          $stmt= $pdo->prepare($sql);
          $stmt->execute();
          
        }

    } else {

        $resuls = $pdo->query("SELECT id, Exterior, Medio, PuertaA, PuertaB FROM Puertas WHERE id=1 LIMIT 1")->fetch();

        $Exterior= $resuls['Exterior'];
        $Medio= $resuls['Medio'];
        $PuertaA= $resuls['PuertaA'];
        $PuertaB= $resuls['PuertaB'];  

  		echo "#>" .$Exterior . "," . $Medio . "," . $PuertaA . "," . $PuertaB . "<#" ; 

        if (isset($_GET['onlyCheck']) == false) {
            $sql = "";

            if ($Exterior == 1){
                $sql = "Exterior=0";

            }
            if ($Medio == 1){
                if ($sql != "") $sql = $sql . ",";
                $sql = $sql .  "Medio=0";

            }
         
          
            if ($PuertaA == 1){
                if ($sql != "") $sql = $sql . ",";
                $sql = $sql . "PuertaA=0";

            }

            if ($PuertaB == 1){
                if ($sql != "") $sql = $sql .  ",";
                $sql = $sql .  "PuertaB=0";

            } 

 
            if ( $sql != "") {
                $sql = "UPDATE Puertas SET " . $sql . " WHERE id=1";

              
                $stmt= $pdo->prepare($sql);
                $stmt->execute();
              
                $publisher_id = $pdo->lastInsertId();
            }
        }
            
    }
    
    mysqli_close($conn); 

?>