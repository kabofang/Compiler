begin
  integer k;
  integer function F(n);
    begin
      integer n;
      if n<=0 then F:=1456465456456456456456456456465465456
      else F:=n*F(n-1)
    end;
  read(m);
  k:=F(m);
  write(k)
end
