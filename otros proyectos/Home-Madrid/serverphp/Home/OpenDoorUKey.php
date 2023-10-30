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

        $linkUnicoUso = 'LinkUnSoloUso.php?claveID=' . $claveID;

      ?>


<!DOCTYPE html>
<html>
   <head>
      <!-- Basic Page Needs
         ================================================== -->
      <meta charset="utf-8">
      <title>Sistema Open Door - Pablo</title>
      <!-- Mobile Specific Metas
         ================================================== -->
      <meta http-equiv="X-UA-Compatible" content="IE=edge">
      <meta name="description" content="Sistema Open Door - Pablo">
      <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=5.0">
      <meta name="author" content="Pablo Schmidt">
      <!-- Favicon -->
      <link rel="shortcut icon" type="image/x-icon" href="images/favicon.png" />
      <!-- PLUGINS CSS STYLE -->
      <link rel="stylesheet" href="plugins/bootstrap/bootstrap.min.css">
      <link rel="stylesheet" href="plugins/themify-icons/themify-icons.css">
      <link rel="stylesheet" href="plugins/slick/slick.css">
      <link rel="stylesheet" href="plugins/slick/slick-theme.css">
      <link rel="stylesheet" href="plugins/fancybox/jquery.fancybox.min.css">
      <link rel="stylesheet" href="plugins/aos/aos.css">
      <!-- CUSTOM CSS -->
      <link href="css/style.css" rel="stylesheet">
   </head>
   <body class="body-wrapper" data-spy="scroll" data-target=".privacy-nav">
      <nav class="navbar main-nav navbar-expand-lg px-2 px-sm-0 py-2 py-lg-0">
         <div class="container">
            <a class="navbar-brand" href="#"><img src="images/logo.png" alt="logo"></a>
            
         </div>
      </nav>

      <section class="section gradient-banner">
         <div class="container">
            <div class="row align-items-center">
               <div class="col-md-6 order-2 order-md-1 text-center text-md-left">
                  <h1 class="text-white font-weight-bold mb-4">Sistema de apertura de puertas</h1>

                  <p class="text-white mb-5">Abriendo puerta... Atencion!!! Ten cuidado... </p>




      <section class="call-to-action-app section">
      <div class="container">
        <div class="row">
          <div class="col-lg-12">
            <ul class="list-inline">
              
              <li class="list-inline-item">
              <a href="<?php echo $linkUnicoUso; ?>" class="btn btn-rounded-icon">Continuar a la siguiente puerta -> </a>
              </li>

            </ul>
          </div>

        </div>
      </div>
      </section>


               </div>
            </div>
          </div>
      </section>



   </body>
</html>

      <?php

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