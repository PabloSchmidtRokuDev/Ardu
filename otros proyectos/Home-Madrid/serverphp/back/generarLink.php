<?php

/*
  listar todos los posts o solo uno
 */

if ($_SERVER['REQUEST_METHOD'] == 'GET')
{
  //Exterior, Medio, PuertaA, PuertaB

    $Exterior = 0;
    $Medio = 0;
    $PuertaA = 0;
    $PuertaB = 0;

    $generarLink = false;

    if (isset($_GET['Exterior']))
    {
      $Exterior = $_GET['Exterior'];
      $generarLink = true;
    }

    if (isset($_GET['Medio']))
    {
      $Medio = $_GET['Medio'];
      $generarLink = true;
    }

    if (isset($_GET['PuertaA']))
    {
      $PuertaA = $_GET['PuertaA'];
      $generarLink = true;
    }

    if (isset($_GET['PuertaB']))
    {
      $PuertaB = $_GET['PuertaB'];
      $generarLink = true;
    }

    if ($generarLink == true){

      $dbhost = 'db5007242110.hosting-data.io';
      $dbuser = 'dbu1168746';
      $dbpass = 'qwe159ZXC';
         
      $database = "dbs5969492";
         
      $pdo = new PDO("mysql:host=$dbhost;dbname=$database", $dbuser, $dbpass);


      $aleatorio = rand(1111111, 9999999);

      $DateAndTime = date('mdYhis', time());

      $aleatorio = $aleatorio . $DateAndTime . rand(1111111, 9999999);

      $sql = "INSERT INTO LinkUnicoUso(EntradaCount, MedioCount, PuertaACount, PuertaBCount, claveID) VALUES ($Exterior, $Medio, $PuertaA, $PuertaB, $aleatorio)";
    
      $stmt= $pdo->prepare($sql);
      $stmt->execute();
      
      $publisher_id = $pdo->lastInsertId();

       mysqli_close($conn);
     
        header("HTTP/1.1 200 OK");
      
      ?>

<!DOCTYPE html>
  <html>
  <head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
  .btn {
  border: none;
  background-color: #CCC;
  font-size: 16px;
  cursor: pointer;
  display: inline-block;
  margin-right: 2px;
  margin-left: 2px;
  padding-top: 14px;
  padding-right: 28px;
  padding-bottom: 14px;
  padding-left: 28px;
  }
.btn:hover {background: #eee;}
.success {color: green;}
  .info {color: dodgerblue;}
  .warning {color: orange;}
  .danger {color: red;}
  .default {color: black;}
  </style> </head> <body>
<h2>Se senero el Link para abrir las puertas:</h2>

  <a href="http://devpablo.com/Home/LinkUnSoloUso.php?codigo=<?php echo $aleatorio; ?>">
    http://devpablo.com/Home/LinkUnSoloUso.php?codigo=<?php echo $aleatorio; ?>
  </a>
</br>
<button id="btn" onclick="copyToClickBoard()">Copy</button>
</br>
<script>
  
  function copyToClickBoard(){
    var content = "http://devpablo.com/Home/LinkUnSoloUso.php?codigo=<?php echo $aleatorio; ?>";

    navigator.clipboard.writeText(content)
        .then(() => {
        alert("Copied!");
    })
        .catch(err => {
        alert("error!");
    })
 
}

</script> 

</br>

<?php 
  if ($Exterior > 0) { 
  echo "<h3>Entrada:" . $Exterior . " disponibles  </h3>";
  echo "</br>";
  }

  if ($Medio > 0) { 
  echo "<h3>Medio:" . $Medio . " disponibles  </h3>";
  echo "</br>";
  }

  if ($PuertaA > 0) { 
  echo "<h3>$Puerta A:" . $PuertaA . " disponibles  </h3>";
  echo "</br>";
  }

  if ($PuertaB > 0) { 
  echo "<h3>$Puerta B:" . $PuertaB . " disponibles  </h3>";
  echo "</br>";
  }

  exit();
} else {

       header("HTTP/1.1 200 OK");
        echo json_encode('Error de id ');
        exit();
       }
  
}

?>
</body> </html>

<?php 

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