; ModuleID = '/home/sichao/Assignments/assignment-4-cflr-hanstechnow/Assignment-4-CFLR/Test-Cases/ptr-dereference2.c'
source_filename = "/home/sichao/Assignments/assignment-4-cflr-hanstechnow/Assignment-4-CFLR/Test-Cases/ptr-dereference2.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32**, align 8
  %3 = alloca i32*, align 8
  %4 = alloca i32*, align 8
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  store i32 0, i32* %1, align 4
  store i32 10, i32* %5, align 4
  store i32** %3, i32*** %2, align 8
  store i32* %5, i32** %3, align 8
  %7 = load i32**, i32*** %2, align 8
  %8 = load i32*, i32** %7, align 8
  store i32* %8, i32** %4, align 8
  %9 = load i32*, i32** %4, align 8
  %10 = load i32, i32* %9, align 4
  store i32 %10, i32* %6, align 4
  %11 = load i32*, i32** %4, align 8
  %12 = bitcast i32* %11 to i8*
  %13 = bitcast i32* %5 to i8*
  call void @MUSTALIAS(i8* noundef %12, i8* noundef %13)
  %14 = load i32*, i32** %4, align 8
  %15 = bitcast i32* %14 to i8*
  %16 = load i32*, i32** %3, align 8
  %17 = bitcast i32* %16 to i8*
  call void @MUSTALIAS(i8* noundef %15, i8* noundef %17)
  ret i32 0
}

declare void @MUSTALIAS(i8* noundef, i8* noundef) #1

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 1}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"Ubuntu clang version 14.0.0-1ubuntu1.1"}
