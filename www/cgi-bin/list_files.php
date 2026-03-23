<?php
$uploadDir = '../upload/';

if (!is_dir($uploadDir)) {
    header('Content-Type: application/json');
    echo json_encode([]);
    exit;
}

$files = array_values(array_diff(scandir($uploadDir), array('.', '..')));

header('Content-Type: application/json');
echo json_encode($files);
?>