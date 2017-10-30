for i in $(seq 1 $2); do (./cliente 127.0.0.1 $1 < input > out/$i &) ; done
