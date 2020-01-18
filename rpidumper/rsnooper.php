<?php

$content = $_FILES["rdata"];
echo $content["name"] . " ";
echo $content["type"] . " ";
echo $content["size"] . " ";
echo $content["tmp_name"] . " ";

move_uploaded_file($_FILES['rdata']['tmp_name'], $content["name"]);


?>