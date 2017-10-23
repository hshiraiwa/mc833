for i in $(seq 1 $1); do (./cliente 127.0.0.1 8081 < input > out/$i &) ; done
