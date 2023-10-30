<?php
$codigoCorrecto = false;

if (isset($_GET['claveID']))
{
    $claveID = $_GET['claveID'];

    $dbhost = 'db5007242110.hosting-data.io';
    $dbuser = 'dbu1168746';
    $dbpass = 'qwe159ZXC';

    $database = "dbs5969492";

    $pdo = new PDO("mysql:host=$dbhost;dbname=$database", $dbuser, $dbpass);

    $resuls = $pdo->query("SELECT * FROM LinkUnicoUso WHERE claveID=$claveID LIMIT 1")
        ->fetch();


        $Exterior = $resuls['EntradaCount'];
        $Medio = $resuls['MedioCount'];
        $PuertaA = $resuls['PuertaACount'];
        $PuertaB = $resuls['PuertaBCount'];

        if ($Exterior > 0 || $Medio > 0 || $PuertaA > 0 || $PuertaB > 0){
          $codigoCorrecto = true;
        }
      
      //echo $Exterior . " - ".  $Medio. " - ".$PuertaA . " - ". $PuertaB;
    
    mysqli_close($conn);

}

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

                     <?php
if ($codigoCorrecto != true)
{
?>
<p class="text-white mb-5">El codigo no se encuentra disponible :(</p>

  <?php
}
else
{

?>

   <p class="text-white mb-5">Las siguientes opciones abren las diferentes puertas. Utilice las opciones cuando este delante de la puerta</p>

         

      <section class="call-to-action-app section">
      <div class="container">
        <div class="row">
          <div class="col-lg-12">
            <ul class="list-inline">

              <?php
        if ($Exterior > 0)
        {
    ?>
    
              <li class="list-inline-item">
              <a class="btn btn-rounded-icon" href="http://devpablo.com/Home/OpenDoorUKey.php?Exterior=1&claveID=<?php echo $claveID; ?>">Entrada principal del edificio</a>



              </li>
            <?php
    }

        if ($Medio > 0)
        {
    ?>
              <li class="list-inline-item">
              <a class="btn btn-rounded-icon" href="http://devpablo.com/Home/OpenDoorUKey.php?Medio=1&claveID=<?php echo $claveID; ?>">Segunda puerta (segundo piso escalera)</a>
              </li>

  <?php
    }

    if ($PuertaA > 0)
    {
?>
              <li class="list-inline-item">
              <a class="btn btn-rounded-icon" href="http://devpablo.com/Home/OpenDoorUKey.php?PuertaA=1&claveID=<?php echo $claveID; ?>">Puerta A</a>
              </li>

                <?php
    }

    if ($PuertaB > 0)
    {
?>
              <li class="list-inline-item">
              <a class="btn btn-rounded-icon" href="http://devpablo.com/Home/OpenDoorUKey.php?PuertaB=1&claveID=<?php echo $claveID; ?>">Puerta B</a>
              </li>

              <?php
    }
}
?>
            </ul>
          </div>

        </div>
      </div>
      </section>


                </div>
            </div>
      </section>



   </body>
</html>


