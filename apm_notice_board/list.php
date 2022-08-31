<?php
    require_once($_SERVER['DOCUMENT_ROOT'].'/top.php');
    include_once($_SERVER['DOCUMENT_ROOT'].'/header.php');
?>

<!--
<div class=ListDivStyle>
    <a href="http://<?=$_SERVER['HTTP_HOST']?>/write.php" class=Listbtn>글쓰기</a>
</div>
<div class=ListDivStyle>
    <a href="http://<?=$_SERVER['HTTP_HOST']?>/list.php" class=Listbtn>목록</a>
</div>
<br><br><br>
-->
<?php
    $sql = "SELECT * FROM bbs";
    $query_result = $mysqli->query($sql);
    $total_record = $query_result->num_rows;
?>
<?php if($total_record == 0) : ?>
    글이 없습니다
<?php
    else :
        if(isset($page))
        {
            $now_page = $page;
        }
        else
        {
            $now_page = 1;
        }

        // 한 페이지에 보여질 글 수
        $show_post_count = 10;

        // 보여질 글 수 x (현재 페이지 - 1) = 화면에 보여질 시작할 글 record
        // 5 * 4 = 20
        $start_record = $show_post_count * ($now_page - 1);
        //echo "start record : ".$start_record."<br>";

        // end_reccord = 5
        $end_record = $show_post_count;
        //echo "end record : ".$end_record."<br>";

        // 시작할 글 record + 5한 값이 bbs 테이블의 record 수보다 많을 경우
        // 즉, 전체 글 수 보다 더 많이 출력되려고 할 경우 
        // 20 + 5 = 25 > 21
        if(($start_record + $end_record) > $total_record)
        {
            // 전체 글 수에서 시작 글 수의 값만큼 뺀다.
            // 23 - 20 = 3
            $end_record = $total_record - $start_record;
            //echo "start + end > total : ".$end_record."<br>";
        }

        //echo "start_record : ".$start_record."<br>";
        //echo "end_record : ".$end_record."<br>";


        $sql = "select * from bbs where 1 order by doc_idx desc limit $start_record, $end_record";
        $query_result = $mysqli->query($sql);
?>


<table class=ListTableStyle>
    <thead>
        <tr class=ListTrStyle>
            <th scope="col" class=ListThStyle>글번호</th>
            <th scope="col" class=ListThStyle>제목</th>
            <th scope="col" class=ListThStyle>작성자 인덱스</th>
            <th scope="col" class=ListThStyle>글쓴이</th>
            <th scope="col" class=ListThStyle>날짜</th>
            <th scope="col" class=ListThStyle>파일</th>
            <th scope="col" class=ListThStyle>조회수</th>
        </tr>
    </thead>
    <?php
        while($data = $query_result->fetch_array()) :
    ?>
    <tr style="border:1px solid #bcbcbc";>
        <td scope="row" class=ListTdStyle><?=$data['doc_idx']?></td>
        <td scope="row" class=ListTdStyle><a href="http://<?= $_SERVER['HTTP_HOST'];?>/view.php?doc_idx=<?=$data['doc_idx'];?>"><?=$data['subject']?></a></td>
        <td scope="row" class=ListTdStyle><?=$data['member_idx']?></td>
        <td scope="row" class=ListTdStyle><?=$data['writer']?></td>
        <td scope="row" class=ListTdStyle><?=date("Y:m:d H:i:s",$data['reg_date']);?></td>
        <td scope="row" class=ListTdStyle></td>
        <td scope="row" class=ListTdStyle><?=$data['views']?></td>
    </tr>
    <?php endwhile ?>
</table>

<?php
    endif
?>

<div>
    <ul class=ListUlStyle>
<?php

    // 현재 블럭 구하기
    // 현재 페이지 / [최대 보여질 블럭 수] = 현재 블럭 
    $show_block_count= 2;
    $now_block = ceil($now_page / $show_block_count);
    //echo "now_block : ".$now_block."<br>";

    // 전체 페이지 구하기
    // 전체 글 수 / 보여질 최대 글 수
    $total_page = ceil($total_record / $show_post_count);
    //echo "total_page : ".$total_page."<br>";

    // 한 세트에 블럭이 4개인 세트가 몇 개인지(1,2,3,4가 한 세트, 5,6,7,8이 한 세트)
    // 전체 페이지 / 4
    $total_block_set = ceil($total_page / $show_block_count);
    //echo "total_block_set : ".$total_block_set."<br>";

    // 현재 블럭 세트가 2 이상이면
    if(1 < $now_block)
    {
        // (현재 블럭 - 1) x 한 블럭에 보여질 최대 블럭 수 
        //echo "now_block : ".($now_block - 1)."<br>";

        $pre_page = ($now_block - 1) * $show_block_count-1;
        //echo "pre_page : ".$pre_page."<br>";

        echo '<a href="http://'.$_SERVER['HTTP_HOST'].'/list.php?block='.$pre_page.'">이전</a>';
    }

    $start_page = ($now_block - 1) * $show_block_count + 1;
    $end_page = $now_block * $show_block_count;
    if($end_page > $total_page)
    {
        $end_page = $total_page;
    }
?>

<?php for($i = $start_page; $i<=$end_page; $i++):?>
    <li class=ListLiStyle><a href="./list.php?page=<?=$i;?>"><?=$i;?></a></li>
<?php endfor ?>
    </ul>

<?php
    // 현재 블럭이 전체 블럭보다 작으면
    if($now_block < $total_block_set)
    {
        $post_page = $now_block * $show_block_count + 1;
        echo '<a href="http://'.$_SERVER['HTTP_HOST'].'/list.php?page='.$post_page.'">다음</a>';
    }
?>
</div>
<?php
    include_once($_SERVER['DOCUMENT_ROOT'].'/footer.php');
?>
