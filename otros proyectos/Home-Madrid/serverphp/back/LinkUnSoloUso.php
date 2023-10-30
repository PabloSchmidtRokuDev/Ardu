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

    $resuls = $pdo->query("SELECT * FROM LinkUnicoUso WHERE claveID=1 LIMIT 1")
        ->fetch();

    if ($resultado->fetchColumn() > 0) {
        $Exterior = $resuls['Exterior'];
        $Medio = $resuls['Medio'];
        $PuertaA = $resuls['PuertaA'];
        $PuertaB = $resuls['PuertaB'];

        if ($Exterior > 0 || $Medio > 0 || $PuertaA > 0 || $PuertaB > 0){
          $codigoCorrecto = true;
        }
    }
    mysqli_close($conn);

}

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
  </style>
  </head>
  <body>

    <?php
if ($codigoCorrecto != true)
{
?>

  <h2>El codigo no se encuentra disponible :( </h2>

  <?php
}
else
{

?>
<h2>Solo precione el boton para abrir la puerta cuando este delante de la misma. </h2>
<h2>La puerta puede tener una demora de 5 segundo aproximadamente en abrirse</h2>
</br>
    </br>
<?php
    if ($Exterior > 0)
    {
?>
  <a href="http://devpablo.com/Home/OpenDoorUKey.php?Exterior=1&claveID=<?php echo $claveID; ?>">
    <button class="btn success">Acceso Edificio</button>
  </a>
    </br>
    </br>
  <?php
    }

    if ($Medio > 0)
    {
?>

  <a href="http://devpablo.com/Home/OpenDoorUKey.php?Medio=1&claveID=<?php echo $claveID; ?>">
    <button class="btn warning">Abrir Medio</button>
  </a>
</br>
</br>

  <?php
    }

    if ($PuertaA > 0)
    {
?>

  <a href="http://devpablo.com/Home/OpenDoorUKey.php?PuertaA=1&claveID=<?php echo $claveID; ?>">
    <button class="btn danger">Abrir Puerta A</button>
  </a>
</br>
</br>

  <?php
    }

    if ($PuertaB > 0)
    {
?>

  <a href="http://devpablo.com/Home/OpenDoorUKey.php?PuertaB=1&claveID=<?php echo $claveID; ?>">
    <button class="btn default">Abrir Puerta B</button>
  </a>

<?php
    }
}
?>
</body>
  </html>
