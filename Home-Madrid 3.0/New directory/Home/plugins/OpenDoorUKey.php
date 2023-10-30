<?php



if ($_SERVER['REQUEST_METHOD'] == 'GET')
{
  //Exterior, Medio, PuertaA, PuertaB

    $Exterior = 0;
    $Medio = 0;
    $PuertaA = 0;
    $PuertaB = 0;

    $openDoor = false;

    if (isset($_GET['claveID']))
    {

            $dbhost = 'db5007242110.hosting-data.io';
      $dbuser = 'dbu1168746';
      $dbpass = 'qwe159ZXC';
         
      $database = "dbs5969492";
         
      $pdo = new PDO("mysql:host=$dbhost;dbname=$database", $dbuser, $dbpass);


      $claveID = $_GET['claveID'];
    

    if (isset($_GET['Exterior']))
    {
      $Exterior = $_GET['Exterior'];

      $sqlLinkUnicoUso = "UPDATE LinkUnicoUso SET EntradaCount=EntradaCount-1 WHERE claveID = $claveID";
    
      $openDoor = true;

    } else if (isset($_GET['Medio']))
    {
      $Medio = $_GET['Medio'];

      $sqlLinkUnicoUso = "UPDATE LinkUnicoUso SET MedioCount=MedioCount -1 WHERE claveID = $claveID";

      $openDoor = true;

    } else if (isset($_GET['PuertaA']))
    {
      $PuertaA = $_GET['PuertaA'];

      $sqlLinkUnicoUso = "UPDATE LinkUnicoUso SET PuertaACount=PuertaACount-1 WHERE claveID = $claveID";

      $openDoor = true;

    } else if (isset($_GET['PuertaB']))
    {
      $PuertaB = $_GET['PuertaB'];
      $sqlLinkUnicoUso = "UPDATE LinkUnicoUso SET PuertaBCount=PuertaBCount-1 WHERE claveID = $claveID";

      $openDoor = true;
    }

}
    if ($openDoor == true){

      $stmt= $pdo->prepare($sqlLinkUnicoUso);
      $stmt->execute();

      $sql = "UPDATE Puertas SET Exterior=$Exterior,Medio=$Medio,PuertaA=$PuertaA,PuertaB=$PuertaB WHERE id = 1";
    
      $stmt= $pdo->prepare($sql);
      $stmt->execute();
      
      $publisher_id = $pdo->lastInsertId();

       mysqli_close($conn);

        $linkUnicoUso = 'http://devpablo.com/Home/LinkUnSoloUso.php?claveID=' . $claveID;

      ?>

      <h3>Abriendo puerta!!! atencion... ten cuidado...</h3>

      <?php
      echo $linkUnicoUso;

        include $linkUnicoUso;
        exit();
      } else {

       header("HTTP/1.1 200 OK");
        echo json_encode('Error de id ');
        exit();
       }
  
}

// Crear un nuevo post
if ($_SERVER['REQUEST_METHOD'] == 'POST')
{
  $log = "start - ";
  $body = json_decode(file_get_contents("php://input"), true);
  //$body = json_encode($body);

$log = $log. "  _ body->{'rokuid'} - ". $body['rokuid'];

  $nombre_fichero = "LogsTestRoku/". $body['rokuid']. ".txt";


$log = $log. "  - nombre_fichero - ". $nombre_fichero;

  if (file_exists($nombre_fichero)) {
    $log = $log. "  -- abre el fichero con a - ";
      $fp = fopen($nombre_fichero, "a");
  } else {
    $log = $log. "  -- crea el fichero con W - ";
      $fp = fopen($nombre_fichero, "w");
  }


  fwrite($fp, json_encode($body['msj']). PHP_EOL);

  fclose($fp);

$log = $log. "  --- close file";

      header("HTTP/1.1 200 OK");
     echo json_encode($log);
      exit();

}
//Borrar
if ($_SERVER['REQUEST_METHOD'] == 'DELETE')
{
  // $id = $_GET['id'];
 //  $statement = $dbConn->prepare("DELETE FROM posts where id=:id");
 //  $statement->bindValue(':id', $id);
 //  $statement->execute();
  header("HTTP/1.1 200 OK");
  exit();
}
//Actualizar
if ($_SERVER['REQUEST_METHOD'] == 'PUT')
{
    // $input = $_GET;
    // $postId = $input['id'];
    // $fields = getParams($input);
    // $sql = "
    //       UPDATE posts
    //       SET $fields
    //       WHERE id='$postId'
    //        ";
    // $statement = $dbConn->prepare($sql);
    // bindAllValues($statement, $input);
    // $statement->execute();
    header("HTTP/1.1 200 OK");
    exit();
}
//En caso de que ninguna de las opciones anteriores se haya ejecutado
header("HTTP/1.1 400 Bad Request");
?>