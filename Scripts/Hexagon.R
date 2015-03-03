library(ggplot2)

DrawHexagon <-
    function(data){
        ggplot(data, aes(x=V2,y=V3,colour=V1, shape=V1)) +
            geom_point() +
            scale_x_continuous(limits=c(-1,1)) +
            scale_y_continuous(limits=c(-1,1)) +
            geom_segment(x=-0.866, xend=0, y=0.5, yend=1, colour="black") +
            geom_segment(x=-0.866, xend=0, y=-0.5, yend=-1, colour="black") +
            geom_segment(x=0, xend=0.866, y=-1, yend=-0.5, colour="black") +
            geom_segment(x=0, xend=0.866, y=1, yend=0.5, colour="black") +
            geom_segment(x=-0.866, xend=-0.866, y=-0.5, yend=0.5, colour="black") +
            geom_segment(x=0.866, xend=0.866, y=-0.5, yend=0.5, colour="black") +
            theme_bw() +
            theme(
                panel.grid=element_blank(),
                axis.ticks=element_blank(),
                axis.text=element_blank(),
                axis.title=element_blank(),
                legend.title=element_blank()
            )
    }

args <- commandArgs(trailingOnly = TRUE)
x<-read.table(file=args[1], header=F, sep="\t")
png(args[2], bg='transparent', res = 300, width=15, height=10, units='cm')
DrawHexagon(x)
dev.off()
