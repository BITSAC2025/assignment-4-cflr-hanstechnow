; ModuleID = '/home/sichao/Assignments/assignment-4-cflr-hanstechnow/Assignment-4-CFLR/Test-Cases/heap-linkedlist.c'
source_filename = "/home/sichao/Assignments/assignment-4-cflr-hanstechnow/Assignment-4-CFLR/Test-Cases/heap-linkedlist.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.Node = type { i32*, %struct.Node* }

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @malloc_list(%struct.Node* noundef %0, i32 noundef %1) #0 {
  %3 = alloca %struct.Node*, align 8
  %4 = alloca i32, align 4
  %5 = alloca i32*, align 8
  %6 = alloca i32*, align 8
  %7 = alloca %struct.Node*, align 8
  store %struct.Node* %0, %struct.Node** %3, align 8
  store i32 %1, i32* %4, align 4
  store i32* null, i32** %5, align 8
  store i32* null, i32** %6, align 8
  store %struct.Node* null, %struct.Node** %7, align 8
  %8 = load %struct.Node*, %struct.Node** %3, align 8
  %9 = getelementptr inbounds %struct.Node, %struct.Node* %8, i32 0, i32 0
  store i32* null, i32** %9, align 8
  br label %10

10:                                               ; preds = %13, %2
  %11 = load i32, i32* %4, align 4
  %12 = icmp ne i32 %11, 0
  br i1 %12, label %13, label %31

13:                                               ; preds = %10
  %14 = call noalias i8* @malloc(i64 noundef 4) #3
  %15 = bitcast i8* %14 to i32*
  %16 = load %struct.Node*, %struct.Node** %3, align 8
  %17 = getelementptr inbounds %struct.Node, %struct.Node* %16, i32 0, i32 0
  store i32* %15, i32** %17, align 8
  %18 = load %struct.Node*, %struct.Node** %3, align 8
  %19 = getelementptr inbounds %struct.Node, %struct.Node* %18, i32 0, i32 0
  %20 = load i32*, i32** %19, align 8
  store i32* %20, i32** %5, align 8
  %21 = call noalias i8* @malloc(i64 noundef 16) #3
  %22 = bitcast i8* %21 to %struct.Node*
  store %struct.Node* %22, %struct.Node** %7, align 8
  %23 = load %struct.Node*, %struct.Node** %7, align 8
  %24 = load %struct.Node*, %struct.Node** %3, align 8
  %25 = getelementptr inbounds %struct.Node, %struct.Node* %24, i32 0, i32 1
  store %struct.Node* %23, %struct.Node** %25, align 8
  %26 = load %struct.Node*, %struct.Node** %3, align 8
  %27 = getelementptr inbounds %struct.Node, %struct.Node* %26, i32 0, i32 0
  %28 = load i32*, i32** %27, align 8
  store i32* %28, i32** %6, align 8
  %29 = load i32, i32* %4, align 4
  %30 = add nsw i32 %29, -1
  store i32 %30, i32* %4, align 4
  br label %10, !llvm.loop !6

31:                                               ; preds = %10
  %32 = load i32*, i32** %5, align 8
  %33 = bitcast i32* %32 to i8*
  %34 = load i32*, i32** %6, align 8
  %35 = bitcast i32* %34 to i8*
  call void @MAYALIAS(i8* noundef %33, i8* noundef %35)
  %36 = load %struct.Node*, %struct.Node** %7, align 8
  %37 = bitcast %struct.Node* %36 to i8*
  %38 = load i32*, i32** %5, align 8
  %39 = bitcast i32* %38 to i8*
  call void @NOALIAS(i8* noundef %37, i8* noundef %39)
  ret void
}

; Function Attrs: nounwind
declare noalias i8* @malloc(i64 noundef) #1

declare void @MAYALIAS(i8* noundef, i8* noundef) #2

declare void @NOALIAS(i8* noundef, i8* noundef) #2

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca %struct.Node*, align 8
  %3 = alloca i32, align 4
  store i32 0, i32* %1, align 4
  %4 = call noalias i8* @malloc(i64 noundef 16) #3
  %5 = bitcast i8* %4 to %struct.Node*
  store %struct.Node* %5, %struct.Node** %2, align 8
  store i32 4, i32* %3, align 4
  %6 = load %struct.Node*, %struct.Node** %2, align 8
  %7 = load i32, i32* %3, align 4
  call void @malloc_list(%struct.Node* noundef %6, i32 noundef %7)
  %8 = load %struct.Node*, %struct.Node** %2, align 8
  %9 = getelementptr inbounds %struct.Node, %struct.Node* %8, i32 0, i32 1
  %10 = load %struct.Node*, %struct.Node** %9, align 8
  %11 = getelementptr inbounds %struct.Node, %struct.Node* %10, i32 0, i32 0
  %12 = load i32*, i32** %11, align 8
  %13 = bitcast i32* %12 to i8*
  %14 = load %struct.Node*, %struct.Node** %2, align 8
  %15 = getelementptr inbounds %struct.Node, %struct.Node* %14, i32 0, i32 1
  %16 = load %struct.Node*, %struct.Node** %15, align 8
  %17 = getelementptr inbounds %struct.Node, %struct.Node* %16, i32 0, i32 1
  %18 = load %struct.Node*, %struct.Node** %17, align 8
  %19 = bitcast %struct.Node* %18 to i8*
  call void @NOALIAS(i8* noundef %13, i8* noundef %19)
  ret i32 0
}

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 1}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"Ubuntu clang version 14.0.0-1ubuntu1.1"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
