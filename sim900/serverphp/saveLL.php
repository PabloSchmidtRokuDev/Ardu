<?php

/*
  listar todos los posts o solo uno
 */

if ($_SERVER['REQUEST_METHOD'] == 'GET')
{
    if (isset($_GET['Lat']))
    {
   $dbhost = 'db5007240646.hosting-data.io:3036';
   $dbuser = 'dbu2091531';
   $dbpass = 'qwe159ZXC';
   $database = "dbs5967892";
   $conn = mysql_connect($dbhost, $dbuser, $dbpass, $database);
    
    $sql = "INSERT INTO Posicion (Lat, lon, date) VALUES ($_GET['Lat'], $_GET['Lon'], $_GET['Date'])";
    if (mysqli_query($conn, $sql)) {
          echo "New record created successfully";
    } else {
          echo "Error: " . $sql . "<br>" . mysqli_error($conn);
    }
    mysqli_close($conn);

      header("HTTP/1.1 200 OK");
      echo json_encode('servicio get llamado - id recibido: ' + $_GET['id']);

      exit();
	  }
    else {
      //Mostrar lista de post
      //$sql = $dbConn->prepare("SELECT * FROM posts");
      //$sql->execute();
      //$sql->setFetchMode(PDO::FETCH_ASSOC);
      header("HTTP/1.1 200 OK");
      echo json_encode('servicio get llamado - Sin ID ');
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