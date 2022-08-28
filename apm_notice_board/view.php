<?php
    require_once($_SERVER['DOCUMENT_ROOT']."/top.php");
    include_once($_SERVER['DOCUMENT_ROOT']."/header.php");
?>

<?php
    $sql = "select * from bbs where doc_idx= $doc_idx";
    $query_result = $mysqli->query($sql);
    $data = $query_result->fetch_array();
?>

    <table class=ViewTableStyle>
        <tr class=ViewTrStyle>
            <th scope="row" class=ViewTrStyle>작성자</td>
            <td scope="row" class=ViewTdStyle><?=$data['writer'];?></td>
        </tr>
        <tr class=ViewTrStyle>
            <th scope="row" class=ViewTrStyle>제목</td>
            <td scope="row" class=ViewTdStyle><?=$data['subject'];?></td>
        </tr>
        <tr class=ViewTrStyle>
            <th scope="row" class=ViewTrStyle>내용</td>
            <td scope="row" class=ViewTdStyle><?=$data['content'];?></td>
        </tr>
    </table>

    <?php
        //echo "SESSION['member_idx'] : ".$_SESSION['member_idx']."<br>";
        //echo "data['member_idx'] : ".$data['member_idx']."<br>";
        if($_SESSION['member_idx']==$data['member_idx'])
        {
            echo '<br>';
            echo '<div class=ViewDivStyle><a href="http://'.$_SERVER['HTTP_HOST'].'/modify.php?doc_idx='.$doc_idx.'" class=btn>수정</a></div>';
            echo '<div class=ViewDivStyle><a href="http://'.$_SERVER['HTTP_HOST'].'/delete.php?doc_idx='.$doc_idx.'" class=btn>삭제</a></div>';
        }
    ?>
<?php
    include_once($_SERVER['DOCUMENT_ROOT']."/footer.php");
?>