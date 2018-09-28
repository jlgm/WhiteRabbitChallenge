
filter:
	@cd src && g++ filter.cpp -std=c++14 -o filter
	@./src/filter < wordlist > filtered

run:
	@cd src && g++ anagram.cpp -std=c++14 -o anagram -lcrypto
	@./src/anagram e4820b45d2277f3844eac66c903e84be \
				23170acc097c24edb98fc5488ab033fe \
				665e5bcb0c20062fe8abaaf4628bb154 < filtered 

docker-setup: 
	@docker build -t anagram .
	
docker-run:
	@docker run -it --rm anagram