% 1) Nacitaj sidnal zo suboru, vypis vzorkovaciu frekvenciu signalu a jeho
% dlzku vo vzorkoch a v sekundach + pocet reprezentovanych binarnych
% symbolov
fprintf('1)\n');
[s,fs] = audioread('xsvehl09.wav');
s = transpose(s);
s_length = length(s);
fprintf('Vzorkovacia frekvencia signalu je: %f [Hz]. \n', fs);
fprintf('Dĺžka vo vzorkoch: %d [-]. \n', s_length);
fprintf('Dĺžka v sekundách: %f [s]. \n', s_length/fs);
fprintf('Pocet binrnych symbolov: %d [-]. \n\n', s_length/16);

%2) dekoduj s[n]
fprintf('2)\n');
decoded = s(8:16:end);
for i = 1:length(decoded)
    if decoded(i) > 0
        decoded(i) = 1;
    else
        decoded(i) = 0;
    end
end

file = textread('xsvehl09.txt');
file = transpose(file);

comparison = xor(file,decoded);
if (abs(comparison) == 0) 
    fprintf('Dekódovaný signál je totožný so súborom.\n\n');
else    
    fprintf('Dekódovaný signál nie je totožný so súborom.\n\n');
end
t = 0:1/fs:2-1/fs;
t2 = 0:16/fs:2-16/fs;
figure(1);
plot(t,s);
title('Pôvodný signál, jednotlivé vzorky a fltrovaný signál s posunom a bez posunu');
xlabel('t');
ylabel('s(t)');
hold on;
stem(t2,decoded);
hold on;


% 3) filter
fprintf('3)\n');
B = [0.0192 -0.0185 -0.0185 0.0192];
A = [1.0000 -2.8870 2.7997 -0.9113];

if abs(roots(A)) < 1
    fprintf('Filter je stabilný\n\n');
else    
    fprintf('Filter je nestabilný\n\n');
end
figure(2);
subplot(2,1,1);
zplane(B,A);
title('Nulové body a póly filtru');
xlabel('Re');
ylabel('Im');

% 4) frekvenčná charakteristika filtru
fprintf('4)\n');
fn = (0 : 399) / 400;
f_os = fn * fs / 2;
hf = abs(freqz(B, A, 400));
subplot(2,1,2);
plot(f_os,hf);
title('Frekvenčná charakteristika filtru');
xlabel('f[Hz]');
ylabel('|H(f)|');
grid;
fprintf('Filter je podla obrázku typu dolná propusť\n\n');


% 5) filtrovanie signalu filtrom

fprintf('5)\n');
ss= filter(B, A, s);
figure (1);
plot(t,ss);
hold on;
fprintf('Z grafu možno pomocov markerov vyčítať že filtrovaný signál je oneskorený približne o 0,9 [ms]\n\n');


% 6) posunutie filtrovaného signálu
fprintf('6)\n');
fprintf('Dĺžka signálu je 32000 vzorkov a 2 s, hodnote 0,9 ms teda zodpoveda 14 vzorkom.\n\n');
first = ss(15:end);
second = zeros(1, 14);
ssshifted = [first second];
plot(t,ssshifted);
hold off;


% 7) chybovosť filtrovaného signálu
fprintf('7)\n');
decodedsh = ssshifted(8:16:end);
for i = 1:length(decodedsh)
    if decodedsh(i) > 0
        decodedsh(i) = 1;
    else
        decodedsh(i) = 0;
    end
end

errors = sum(xor(decoded,decodedsh));
err_ratio = errors/length(decoded);
fprintf('Počet chýb je %d, chybovosť je teda %.2f [%%].\n\n', errors, err_ratio*100);


% 8) získanie frekvenčnej charakteristiky
fprintf('8)\n');
freq = (0:0.5:(fs-1)/2);
fft_s = abs(fft(s));
fft_s = fft_s(1:s_length/2);
figure(3);
subplot(2,1,1);
plot(freq,fft_s);
title('Spektrum pôvodného signálu');
xlabel('f');
ylabel('|S(f)|');

fft_ss = abs(fft(ss));
fft_ss = fft_ss(1:s_length/2);
subplot(2,1,2);
plot(freq,fft_ss);
title('Spektrum filtrovaného signálu');
xlabel('f');
ylabel('|S(f)|');
%u druheho spektra vidime vplyv filtru. Ten bol typu dolní propusť a podľa 
% očakávania teda potlačil zložky nad frekvenciou cca 450 Hz

% 9)
fprintf('9)\n');
figure(4);
pst=hist(s, 100);
pst=pst/s_length;
subplot(2,1,1);
plot([-0.7425:0.015:0.7425],pst); %hodnota x osi zistená odčítaním z histogramu
title('Funkcia rozdelenia pravdepodobnosti');
xlabel('x');
ylabel('p(x,n)');
subplot(2,1,2);
histogram(s, 100, 'Normalization','probability');
title('Histogram, zobrazujúci funkciu rozdelenia pravdepodobnosti');
xlabel('x');
ylabel('p(x,n)');
fprintf('Kontrola: Integrál nad funkciou rozdelenia pravdepodobnosti má hodnotu: %f.\n\n', sum(pst));


% 10)
%fprintf('10)\n\n');
k = (-50 : 50);
R = xcorr(s) / s_length;
R = R(k + s_length);
figure(5);
plot(k, R);
title('Autokorelačný koeficient');
xlabel('k');
grid;

% 11)
fprintf('11)\n');
fprintf('Indexovanie číslom 1 začína pri -50=>R[0] == R(51)\n');
fprintf('Hodnota koeficientu R[0] je %f.\n', R(51));
fprintf('Hodnota koeficientu R[1] je %f.\n', R(52));
fprintf('Hodnota koeficientu R[16] je %f.\n\n', R(67));

% 12)
fprintf('12)\n');

x = linspace(min(s), max(s), 100);
[h,p,r] = hist2opt(s(1:s_length-1), s(2:s_length), x);
surf(-x,x,p);
axis xy;
colorbar;
xlabel('x2');
ylabel('x1');

% 13) Overenie správnej funkcie hustoty rozdelenia pst
fprintf('\n13)\n');
check = sum(sum(p)) * (x(2) - x(1))^2;
fprintf('Overenie: Koeficient by mal byť 1 a je  %f.\n', check);

% 14)
fprintf('\n14)\n');
fprintf('Hodnota koeficientu R[1] je %f, hodnota z úlohy 11 je %f.\n', r, R(52));
